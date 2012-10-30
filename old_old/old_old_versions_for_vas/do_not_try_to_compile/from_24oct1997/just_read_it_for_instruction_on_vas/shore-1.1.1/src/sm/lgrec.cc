/* --------------------------------------------------------------- */
/* -- Copyright (c) 1994, 1995 Computer Sciences Department,    -- */
/* -- University of Wisconsin-Madison, subject to the terms     -- */
/* -- and conditions given in the file COPYRIGHT.  All Rights   -- */
/* -- Reserved.                                                 -- */
/* --------------------------------------------------------------- */

/*
 *  $Id: lgrec.cc,v 1.59 1997/06/15 03:13:13 solomon Exp $
 */
#define SM_SOURCE
#define LGREC_C
#ifdef __GNUG__
#   pragma implementation
#endif

#include <sm_int_2.h>
#include <lgrec.h>

/*
// LARGE RECORDS
//
// Leaf pages of large records have at most 8120 bytes; the
// rest is overhead on the page.
// Index pages of large records also have at most 8120 bytes.
//
// We have three implementations:
// t_large_0, t_large_1, t_large_2
//
// t_large_0: the body portion of the record in the slotted page
//   is a list of chunks (see lg_tag_chunks_s).  A chunk 
//   represents a set of contiguous pages, up to 65536 pages 
//   long (since 2 bytes are used for the length of a chunk).  Thus, 
//   for a storage manager compiled with 8K pages,
//   a t_large_0 record is between 8096 (largest small object) and 
//   about 21 gigabytes  in size (if each chunk is 65K pages long).
//   Of course, if there is bad clustering in the object, an object
//   might have to be converted to t_large_1 when it grows to only
//   5 pages in size.  
//
// t_large_1, t_large_2: the body portion of the record in the slotted 
//   page is a lg_tag_indirect_s, which contains no information about leaf
//   pages, but points to a root page for the large object, which is 
//   a lgindex_p.
//   Large object index pages use 1 slot, which is simply an array of
//   pids.  Thus, for 8K pages, 2030 pids fit, meaning that a
//   t_large_1 object can be up to about 16.5 megabytes in size, and a
//   t_large_2 object can be up to about 33.5 gigabytes in size.  Since
//   we're not supporting volumes larger than 4 gig, we haven't supported
//   a 3-level tree. 
*/

/* 
 * append adds pages to a large record implemented as
 * a set of chunks.  It returns eFAILURE if the pages cannot be added
 * (implying that the record must be converted to indirect implmentation
 */
rc_t
lg_tag_chunks_h::append(uint num_pages, const lpid_t new_pages[])
{
    FUNC(lg_tag_chunks_h::append);
#ifdef notdef
    if (_cref.chunks[0].npages > 0) {
	// TODO: FOR TESTING lg_tag_chunks_t::append returns eBADAPPEND
	cout << "FOR TESTING lg_tag_chunks_t::append returns eBADAPPEND" << endl;
	W_ERROR(smlevel_0::eBADAPPEND);
    }
#endif
    int		chunk_loc[max_chunks+1]; 
			// marks location of chunks in the list of pages
			// +1 for marking end of chunks
    uint	chunk_count;

    // find the chunks in the list of pages
    chunk_loc[0] = 0;  // first chunk is at 0
    chunk_count = 1;
    uint i;
    for (i = 1; i<num_pages && chunk_count<=max_chunks; i++) {
	shpid_t curr_chunk_len = i - chunk_loc[chunk_count-1];
	if (new_pages[i].page-curr_chunk_len != new_pages[chunk_loc[chunk_count-1]].page) {
	    // new chunk found
	    chunk_loc[chunk_count] = i;
	    chunk_count++;
	}
    }
    if (chunk_count > max_chunks) {
	// too many chunks
	return RC(smlevel_0::eBADAPPEND);
    }
    chunk_loc[chunk_count] = i;  // remember end of chunks to simplify code

    /* 
     * determine if 1st new chunk is contiguous with last page
     * in the large record
     */
    int contig = 0; // 1 indicates 1st new chunk is contiguous
    if (_cref.chunk_cnt == 0 ||
	new_pages[0].page == _last_pid()+1) contig = 1;

    /*
     * See if there are too many chunks for the record type.
     * The contig variable must be ignored if _chunk_cnt == 0. 
     */
    if ((chunk_count + (_cref.chunk_cnt>0 ? _cref.chunk_cnt - contig : 0)) >
	max_chunks) {
	// too many chunks
	return RC(smlevel_0::eBADAPPEND);
    }

    /*
     * update the chunk list with the new chunks
     * beware of a possibly contiguous first chunk
     */
    if (contig) {
	if (_cref.chunk_cnt == 0) {
	    _cref.chunk_cnt = 1;
	    _cref.chunks[0].first_pid = new_pages[0].page;
	    _cref.chunks[0].npages = 0;
	}
	_cref.chunks[_cref.chunk_cnt-1].npages += chunk_loc[1] - chunk_loc[0];
    }
    for (i = contig; i < chunk_count; i++) {
	_cref.chunks[_cref.chunk_cnt-contig+i].first_pid = new_pages[chunk_loc[i]].page;
	_cref.chunks[_cref.chunk_cnt-contig+i].npages = chunk_loc[i+1] - chunk_loc[i];
    }
    w_assert3((_cref.chunk_cnt + chunk_count - contig) <= max_chunks);
    _cref.chunk_cnt += chunk_count - contig;
    DBG(<<"ok");
    return RCOK;
}

/* 
 * truncate() removes pages at the end of large records
 * implemented as a set of chunks.
 */
rc_t
lg_tag_chunks_h::truncate(uint num_pages)
{
    FUNC(lg_tag_chunks_h::truncate);
    int i;
    int first_dealloc = page_count()-num_pages;
    int last_dealloc = page_count()-1;
#ifdef DEBUG
    uint4 check_dealloc = 0;
#endif

    for (i = first_dealloc; i <= last_dealloc; i++) {
	W_DO(smlevel_0::io->free_page(pid(i)));
#ifdef DEBUG
	check_dealloc++;
#endif
    }
    w_assert3(check_dealloc == num_pages);

    for (i = _cref.chunk_cnt-1; i >= 0 && num_pages > 0; i--) {

	if (_cref.chunks[i].npages <= num_pages) {
	    num_pages -= _cref.chunks[i].npages;
	    _cref.chunk_cnt--; // this chunk is not needed
	} else {
	    _cref.chunks[i].npages -= num_pages;
	    num_pages -= num_pages;
	}
    }

    w_assert3(num_pages == 0);
    return RCOK;
}

rc_t
lg_tag_chunks_h::update(uint4 start_byte, const vec_t& data) const
{
    FUNC(lg_tag_chunks_h::update);
    uint4 amount; // amount to update on page
    uint4 pid_count = start_byte/lgdata_p::data_sz; // first page
    uint4 data_size = data.size();

    lpid_t curr_pid(_page.pid().vol(), _cref.store, 0);
    uint4 offset = start_byte%lgdata_p::data_sz;
    uint4 num_bytes = 0;
    while (num_bytes < data_size) {
	amount = MIN(lgdata_p::data_sz-offset, data_size-num_bytes);
	curr_pid.page = _pid(pid_count); 
	lgdata_p lgdata;
	W_DO( lgdata.fix(curr_pid, LATCH_EX) );
	W_DO(lgdata.update(offset, data, num_bytes, amount));
	offset = 0;
	num_bytes += amount;
	pid_count++;
    }
    // verify last page touched is same as calculated last page
    w_assert3(pid_count-1 == (start_byte+data.size()-1)/lgdata_p::data_sz);
    return RCOK;
}

shpid_t lg_tag_chunks_h::_pid(uint4 pid_num) const
{
    FUNC(lg_tag_chunks_h::_pid);
    for (int i = 0; i < _cref.chunk_cnt; i++) {
	if (_cref.chunks[i].npages > pid_num) {
	    return _cref.chunks[i].pid(pid_num);
	}
	pid_num -= _cref.chunks[i].npages;
    }
    W_FATAL(smlevel_0::eINTERNAL);
    return 0;  // keep compiler quiet
}

rc_t
lg_tag_indirect_h::convert(const lg_tag_chunks_h& old_tag)
{
    FUNC(lg_tag_indirect_h::convert);
#ifdef DEBUG
    const int max_pages = 2;
#else
    const int max_pages = 64;
#endif
    w_assert3(_iref.indirect_root == 0);

    lpid_t    page_list[max_pages];

    uint4 old_cnt = old_tag.page_count();

    for (_page_cnt = 0; _page_cnt < old_cnt; _page_cnt += max_pages) {
	uint num_pages;
	for (num_pages = 0; num_pages < MIN(max_pages, old_cnt-_page_cnt); num_pages++ ) {
		page_list[num_pages] = old_tag.pid(_page_cnt+num_pages);
	}
	W_DO(append(num_pages, page_list));
    }
    return RCOK;
}

rc_t
lg_tag_indirect_h::append(uint num_pages, const lpid_t new_pages[])
{
    FUNC(lg_tag_indirect_h::append);
    const uint max_pages = 64;
    shpid_t   page_list[max_pages];
    w_assert3(num_pages <= max_pages);
    for (uint i=0; i<num_pages; i++) page_list[i]=new_pages[i].page;

    if (_iref.indirect_root == 0) {
	// allocate a root indirect page, near last page in store
	lpid_t root_pid;
	W_DO(smlevel_0::io->last_page(stid(), root_pid));
	W_DO(smlevel_0::io->alloc_pages(stid(), root_pid, 1, &root_pid));
	_iref.indirect_root = root_pid.page;
	lgindex_p root;
	W_DO( root.fix(root_pid, LATCH_EX, root.t_virgin) ); 
	// perform fake read of the new page
    }

    // calculate the number of pages to append to last index page
    uint space_on_last = lgindex_p::max_pids-
			_pages_on_last_indirect();
    uint4 pages_on_last = MIN(num_pages, space_on_last);

    // number of pages to place on a new indirect_page
    uint4 pages_on_new = num_pages - pages_on_last;

    // append pages to 
    lpid_t last_index_pid(stid(), _last_indirect());
    lgindex_p last_index;
    W_DO( last_index.fix(last_index_pid, LATCH_EX) );
    w_assert1(last_index.is_fixed());

    W_DO(last_index.append(pages_on_last, page_list));

    if (pages_on_new) {
	lpid_t new_pid;
	W_DO(_add_new_indirect(new_pid));
	lgindex_p last_index;
	W_DO( last_index.fix(new_pid, LATCH_EX) );
	w_assert1(last_index.is_fixed());
	W_DO(last_index.append(pages_on_new, page_list+pages_on_last));
    }

    return RCOK;
}

/* 
 * truncate() removes pages at the end of large records
 * implemented as indirect blocks 
 */
rc_t
lg_tag_indirect_h::truncate(uint num_pages)
{
    FUNC(lg_tag_indirect_h::truncate);
    int 	i;
    int 	first_dealloc = (int)(_page_cnt-num_pages);
    int 	last_dealloc = (int)(_page_cnt-1);
    recflags_t  rec_type = indirect_type(_page_cnt);

    for (i = first_dealloc; i <= last_dealloc; i++) {
	W_DO(smlevel_0::io->free_page(pid(i)));
    }

    int indirect_rm_count = 0;	  // # indirect pages to remove
    int pids_rm_by_indirect = 0;  // # data pids removed
				  // by removing indirect pages
    int pids_to_rm = 0; 	  // # of pids to rm from last indirect
 
    // indirect pages are only removed if t_large_2
    if (rec_type == t_large_2) {
	uint pids_on_last = _pages_on_last_indirect();
	if (pids_on_last > num_pages) {
	    indirect_rm_count = 0;
	} else {
	    indirect_rm_count = (num_pages-1)/lgindex_p::max_pids+1;
	    pids_rm_by_indirect = pids_on_last+(indirect_rm_count-1)*lgindex_p::max_pids;
	}
    }
    pids_to_rm = num_pages-pids_rm_by_indirect;

    // remove any indirect pages we can
    if (indirect_rm_count > 0) {
	lpid_t	root_pid(stid(), _iref.indirect_root);
	lgindex_p root;
	W_DO( root.fix(root_pid, LATCH_EX) );
	w_assert1(root.is_fixed());

	// first deallocate the indirect pages
	w_assert3(root.pid_count() ==
		_page_cnt/lgindex_p::max_pids + 1);
	int  first_dealloc = root.pid_count()-indirect_rm_count;
	int  last_dealloc = root.pid_count()-1;
	for (i = first_dealloc; i <= last_dealloc; i++) {
	    lpid_t pid_to_free(stid(), root.pids(i));
	    W_DO(smlevel_0::io->free_page(pid_to_free));
	}

	// if will be only one indirect page left, then remember
	// the ID of this page
	/*
	shpid_t new_indirect_root;
	if (indirect_type(_page_cnt-num_pages) == t_large_1) { 
	    new_indirect_root = root->pids(0);
	}
	*/

	W_DO(root.truncate(indirect_rm_count));

	// if there is only one indirect page left, then switch to
	// a single level tree
	if (indirect_type(_page_cnt-num_pages) == t_large_1) { 
	    w_assert3( ((_page_cnt-1)/lgindex_p::max_pids+1) -
		     indirect_rm_count == 0);
	    if (root.pid_count() > 0) {
		// there is at least one page left in the record,
		// the the first page in the root becomes the new root
		_iref.indirect_root = root.pids(0);
	    } else {
		// all pages have been removed, so there is no root
		_iref.indirect_root = 0;
	    }
	    root.unfix();
	    // TODO: WITHOUT THIS FLUSH AN ASSERT WILL FAIL
	    DBG( << "WITHOUT THIS FLUSH AN ASSERT WILL FAIL" );
	    xct()->flush_logbuf();
	    W_DO(smlevel_0::io->free_page(root_pid));
	}
    }

    // if we have not removed all pages, we must truncate pids
    // from the last indirect page
    if (_page_cnt > num_pages) {

	// get the pid for the last indirect page before truncate point
	// temporarily lower _page_cnt
	_page_cnt -= num_pages;
	lpid_t last_index_pid(stid(), _last_indirect());
	_page_cnt += num_pages;

	lgindex_p last_index;
	W_DO( last_index.fix(last_index_pid, LATCH_EX) );
	w_assert1(last_index.is_fixed());
	W_DO(last_index.truncate(pids_to_rm));
    } else {
	w_assert3(_page_cnt == num_pages);
	// we have removed all data pages, so remove the root 
	// (assuming it is not 0 meaning it has already been removed)
	if (_iref.indirect_root != 0) {
	    lpid_t root_pid(stid(), _iref.indirect_root);
	    W_DO(smlevel_0::io->free_page(root_pid));
	    _iref.indirect_root = 0;  // mark that there is no root
	}
    }
    return RCOK;
}

rc_t
lg_tag_indirect_h::update(uint4 start_byte, const vec_t& data) const
{
    FUNC(lg_tag_indirect_h::update);
    uint4 amount; // amount to update on page
    uint   page_to_update = start_byte/lgdata_p::data_sz; // first page
    uint4 data_size = data.size();

    lpid_t curr_pid(stid(), 0);
    uint4 offset = start_byte%lgdata_p::data_sz;
    uint4 num_bytes = 0;
    while (num_bytes < data_size) {
	amount = MIN(lgdata_p::data_sz-offset, data_size-num_bytes);
	curr_pid.page = _pid(page_to_update); 
	lgdata_p lgdata;
	W_DO( lgdata.fix(curr_pid, LATCH_EX) );
	W_DO(lgdata.update(offset, data, num_bytes, amount));
	offset = 0;
	num_bytes += amount;
	page_to_update++;
    }
    w_assert3(page_to_update-1 == (start_byte+data.size()-1)/lgdata_p::data_sz);
    return RCOK;
}

shpid_t lg_tag_indirect_h::_last_indirect()  const
{
    FUNC(lg_tag_indirect_h::_last_indirect);
    if (indirect_type(_page_cnt) == t_large_1) {
	return _iref.indirect_root;
    }

    // read the root page
    lpid_t root_pid(stid(), _iref.indirect_root);
    lgindex_p root;
    W_IGNORE( root.fix(root_pid, LATCH_SH) ); // PAGEFIXBUG
    if (!root.is_fixed()) return 0; 
    
    shpid_t* pids = (shpid_t*)root.tuple_addr(0);
    w_assert3(pids);
    return(pids[(_page_cnt-1)/lgindex_p::max_pids]);
}

rc_t
lg_tag_indirect_h::_add_new_indirect(lpid_t& new_pid)
{
    FUNC(lg_tag_indirect_h::_add_new_indirect);
    // flags for new pages
    w_base_t::uint4_t flags = lgindex_p::t_virgin;

    if (indirect_type(_page_cnt) == t_large_1) {
	// must allocate a new root pid and point it to the current one
	lpid_t root_pid;
	W_DO(smlevel_0::io->last_page(stid(), root_pid));
	W_DO(smlevel_0::io->alloc_pages(stid(), root_pid, 1, &root_pid));

	lgindex_p root;
	W_DO( root.fix(root_pid, LATCH_EX, flags) );
	w_assert1(root.is_fixed());
	W_DO(root.append(1, &_iref.indirect_root));
	w_assert3(root_pid.stid() == stid());
	_iref.indirect_root = root_pid.page;
    }

    // allocate a new page and point to it from the root
    W_DO(smlevel_0::io->last_page(stid(), new_pid));
    W_DO(smlevel_0::io->alloc_pages(stid(), new_pid, 1, &new_pid));
    lgindex_p new_page;
    W_DO( new_page.fix(new_pid, LATCH_EX, flags) );  // format page

    lpid_t root_pid(stid(), _iref.indirect_root);
    lgindex_p root;
    W_DO( root.fix(root_pid, LATCH_EX) );
    w_assert1(root.is_fixed());
    W_DO(root.append(1, &new_pid.page));

    return RCOK;
}

shpid_t lg_tag_indirect_h::_last_pid() const
{
    FUNC(lg_tag_indirect_h::_last_pid);

    lpid_t last_indirect_pid(stid(), _last_indirect());
    if (last_indirect_pid.page == 0) return 0;  // 0-length record
    lgindex_p last_indirect;
    W_IGNORE( last_indirect.fix(last_indirect_pid, LATCH_SH) ); // PAGEFIXBUG
    if (!last_indirect.is_fixed()) return 0;

    return last_indirect.last_pid();
}

shpid_t lg_tag_indirect_h::_pid(uint4 pid_num) const
{
    FUNC(lg_tag_indirect_h::_pid);

    // get the root page
    lpid_t root_pid(stid(), _iref.indirect_root);
    lgindex_p root;
    W_IGNORE( root.fix(root_pid, LATCH_SH) ); // PAGEFIXBUG
    if (!root.is_fixed()) return 0;

    // if the tree is only 1 level, return the correct pid easily
    if (indirect_type(_page_cnt) == t_large_1) {
	return root.pids(pid_num);
    }
    w_assert3(indirect_type(_page_cnt) == t_large_2);

    // find "slot" containing pointer to page with pid_num
    slotid_t idx = pid_num/lgindex_p::max_pids;
    lpid_t indirect_pid(stid(), root.pids(idx));
    lgindex_p indirect;
    W_IGNORE( indirect.fix(indirect_pid, LATCH_SH) ); // PAGEFIXBUG
    if (!indirect.is_fixed()) return 0;

    return indirect.pids(pid_num % lgindex_p::max_pids);
}

void lgdata_p::ntoh()
{
    /*
     *  BUGBUG --- fill in when appropriate 
     */
    W_FATAL(eINTERNAL);
}

rc_t
lgdata_p::format(const lpid_t& pid, tag_t tag, 
	uint4_t flags
	)
{
    w_assert3(tag == t_lgdata_p);

    vec_t vec;  // empty vector
    // format, then create a 0-length slot

    /* Do the formatting and insert w/o logging them */
    W_DO( page_p::format(pid, tag, flags, false) );
    W_COERCE( page_p::insert_expand(0, 1, &vec, false) );


    /* Now, log as one (combined) record: */
    rc_t rc = log_page_format(*this, 0, 1, &vec);

    return rc;
}

rc_t
lgdata_p::append(const vec_t& data, uint4 start, uint4 amount)
{
    FUNC(lgdata_p::append);

    // new vector at correct start and with correct size
    if(data.is_zvec()) {
	W_DO(splice(0, tuple_size(0), 0, zvec_t(amount)));
    } else {
	vec_t new_data(data, u4i(start), u4i(amount));
	w_assert3(amount == new_data.size());
	W_DO(splice(0, tuple_size(0), 0, new_data));
    }
    return RCOK;
}

rc_t
lgdata_p::update(uint4 offset, const vec_t& data, uint4 start,
		 uint4 amount)
{
    FUNC(lgdata_p::update);

    // new vector at correct start and with correct size
    if(data.is_zvec()) {
	W_DO(splice(0, u4i(offset), u4i(amount), zvec_t(amount)));
    } else {
	vec_t new_data(data, u4i(start), u4i(amount));
	w_assert3(amount == new_data.size());
	W_DO(splice(0, u4i(offset), u4i(amount), new_data));
    }
    return RCOK;
}

rc_t
lgdata_p::truncate(uint4 amount)
{
    FUNC(lgdata_p::truncate);

    vec_t       empty;  // zero length vector
    W_DO(splice(0, tuple_size(0)-u4i(amount), u4i(amount), empty));
    return RCOK;
}

rc_t
lgindex_p::format(const lpid_t& pid, tag_t tag, 
	uint4_t flags
	)
{
    w_assert3(tag == t_lgindex_p);

    // create a 0-length slot
    vec_t vec;  // empty vector

    /* Do the formatting and insert w/o logging them */
    W_DO( page_p::format(pid, tag, flags, false) );
    W_COERCE(page_p::insert_expand(0, 1, &vec, false) );

    /* Now, log as one (combined) record: */
    rc_t rc = log_page_format(*this, 0, 1, &vec);
    return rc;
}

rc_t
lgindex_p::append(uint4 num_pages, const shpid_t new_pids[])
{
    // new vector at correct start and with correct size
    vec_t data(new_pids, u4i(num_pages)*sizeof(new_pids[0]));
    W_DO(splice(0, tuple_size(0), 0, data));
    return RCOK;
}

rc_t
lgindex_p::truncate(uint4 num_pages)
{
    vec_t     	empty;  		// zero length vector
    int 	bytes_to_trunc = u4i(num_pages) * sizeof(shpid_t);
    W_DO(splice(0, tuple_size(0)-bytes_to_trunc, bytes_to_trunc, empty));
    return RCOK;
}

void lgindex_p::ntoh()
{
    /*
     *  BUGBUG -- fill in when appropriate
     */
    W_FATAL(eINTERNAL);
}

MAKEPAGECODE(lgdata_p, page_p);
MAKEPAGECODE(lgindex_p, page_p);