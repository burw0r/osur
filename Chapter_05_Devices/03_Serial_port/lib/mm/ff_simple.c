/*!  Dynamic memory allocator - first fit */

#define _FF_SIMPLE_C_
#include <lib/ff_simple.h>

#ifndef ASSERT
#include ASSERT_H
#endif

/*!
 * Initialize dynamic memory manager
 * \param mem_segm Memory pool start address
 * \param size Memory pool size
 * \return memory pool descriptor
*/
void *ffs_init(void *mem_segm, size_t size)
{
	size_t start, end;
	ffs_hdr_t *chunk, *border;
	ffs_mpool_t *mpool;

	ASSERT(mem_segm && size > sizeof(ffs_hdr_t) * 2);

	/* align all on 'size_t' (if already not aligned) */
	start = (size_t) mem_segm;
	end = start + size;
	ALIGN_FW(start);
	mpool = (void *) start;		/* place mm descriptor here */
	start += sizeof(ffs_mpool_t);
	ALIGN(end);

	mpool->first = NULL;

	if (end - start < 2 * HEADER_SIZE)
		return NULL;

	border = (ffs_hdr_t *) start;
	border->size = sizeof(size_t);
	MARK_USED(border);

	chunk = GET_AFTER(border);
	chunk->size = end - start - 2 * sizeof(size_t);
	MARK_FREE(chunk);
	CLONE_SIZE_TO_TAIL(chunk);

	border = GET_AFTER(chunk);
	border->size = sizeof(size_t);
	MARK_USED(border);

	ffs_insert_chunk(mpool, chunk); /* first and only free chunk */


	ffs_hdr_t* xx = mpool->first;
	while(xx != NULL){
		kprintf("-d[%d]\n", xx->size);
		xx = xx->next;
	}

	return mpool;
}

/*!
 * Get free chunk with required size(or slightly bigger)
 * \param mpool Memory pool to be used(if NULL default pool is used)
 * \param size Requested chunk size
 * \return Block address, NULL if can't find adequate free chunk
 */

 /*
 lab3: kod zauzimanja bloka, ako se ne najde blok potrebne velicine
 		 ponovo proci kroz listu blokova i spajati susjedne dok se ne stvori
		 blok koji zadovoljava zahtjev ili dojde do kraja liste
 */
void *ffs_alloc(ffs_mpool_t *mpool, size_t size)
{

	printf("inside alloc\n");
	ffs_hdr_t *iter, *chunk, *pp;

	ASSERT(mpool);

	size += sizeof(size_t) * 2; /* add header and tail size */
	if (size < HEADER_SIZE)
		size = HEADER_SIZE;

	/* align request size to higher 'size_t' boundary */
	ALIGN_FW(size);

	iter = mpool->first;
	while (iter != NULL && iter->size < size)
		iter = iter->next;

	//dosao je do kraja liste i nije nasao dovoljno veliki blok
	/* no adequate free chunk found */
	if (iter == NULL){
		iter = mpool->first;

		int kreirao_dovoljno_velik_blok = 0;
		while (iter->size < size){
			pp = iter;

				while(1){

						ffs_hdr_t* susjed = GET_AFTER(pp);

						//ako je susjed free
						if(CHECK_FREE(susjed)){
							//spoji sa susjedom
							// printf("MERGE blokova\n");
							ffs_remove_chunk(mpool, susjed);
							iter->size += susjed->size;
							// printf("iter.size: %d\n", iter->size);
							CLONE_SIZE_TO_TAIL(iter);

							if(iter->size > size){
								kreirao_dovoljno_velik_blok = 1;
								printf("  [+] dovoljno velik blok kreiran\n");
								break;
							}
				    }
					   else{
								break;
						 }
						pp = susjed;
				}
				// printf("provjera kreirao dovoljno velik blok\n ----> %d\n", kreirao_dovoljno_velik_blok);
				if (kreirao_dovoljno_velik_blok == 1)
					break;
			iter = iter->next;
		}

	}


	if (iter->size >= size + HEADER_SIZE)
	{
		/* split chunk */
		/* first part remains in free list, just update size */
		iter->size -= size;
		CLONE_SIZE_TO_TAIL(iter);

		chunk = GET_AFTER(iter);
		chunk->size = size;
	}
	else { /* give whole chunk */
		chunk = iter;

		/* remove it from free list */
		ffs_remove_chunk(mpool, chunk);
	}

	MARK_USED(chunk);
	CLONE_SIZE_TO_TAIL(chunk);


	ffs_hdr_t* iter2 = mpool->first;
	while(iter2 != NULL){
		kprintf("-d[%d]\n", iter2->size);
		iter2 = iter2->next;
	}


	return ((void *) chunk) + sizeof(size_t);
}

/*!
 * Free memory chunk
 * \param mpool Memory pool to be used(if NULL default pool is used)
 * \param chunk Chunk location(starting address)
 * \return 0 if successful, -1 otherwise
 */

 /*
 lab3: kod operacije free ne spajati oslobodjeni blok sa susjednim
		   vec takav blok samo prebaciti u listu slobodnih
 */
int ffs_free(ffs_mpool_t *mpool, void *chunk_to_be_freed)
{

	printf("inside free\n");
	ffs_hdr_t *chunk;// *before, *after;


	ASSERT(mpool && chunk_to_be_freed);

	chunk = chunk_to_be_freed - sizeof(size_t);
	ASSERT(CHECK_USED(chunk));

	MARK_FREE(chunk); /* mark it as free */

	/* join with left? */
	// before = ((void *) chunk) - sizeof(size_t);
	// if (CHECK_FREE(before))
	// {
	// 	before = GET_HDR(before);
	// 	ffs_remove_chunk(mpool, before);
	// 	before->size += chunk->size; /* join */
	// 	chunk = before;
	// }
	//
	// /* join with right? */
	// after = GET_AFTER(chunk);
	// if (CHECK_FREE(after))
	// {
	// 	ffs_remove_chunk(mpool, after);
	// 	chunk->size += after->size; /* join */
	// }

	/* insert chunk in free list */
	ffs_insert_chunk(mpool, chunk);

	/* set chunk tail */
	CLONE_SIZE_TO_TAIL(chunk);

	ffs_hdr_t* iter = mpool->first;
	while(iter != NULL){
		kprintf("-d[%d]\n", iter->size);
		iter = iter->next;
	}

	return 0;
}

/*!
 * Routine that removes an chunk from 'free' list(free_list)
 * \param mpool Memory pool to be used
 * \param chunk Chunk header
 */
static void ffs_remove_chunk(ffs_mpool_t *mpool, ffs_hdr_t *chunk)
{
	if (chunk == mpool->first) /* first in list? */
		mpool->first = chunk->next;
	else
		chunk->prev->next = chunk->next;

	if (chunk->next != NULL)
		chunk->next->prev = chunk->prev;
}

/*!
 * Routine that insert chunk into 'free' list(free_list)
 * \param mpool Memory pool to be used
 * \param chunk Chunk header
 */
static void ffs_insert_chunk(ffs_mpool_t *mpool, ffs_hdr_t *chunk)
{
	chunk->next = mpool->first;
	chunk->prev = NULL;

	if (mpool->first)
		mpool->first->prev = chunk;

	mpool->first = chunk;
}
