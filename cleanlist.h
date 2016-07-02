#ifndef CLEANLIST_H
#  define CLEANLIST_H

typedef void (*cleaner) (void *);

typedef struct list_item list_item;
struct list_item {
	cleaner func;
	void *item;
	list_item *next;
};

/*
 * Insert new element at the end of the list
 **/
void insert (list_item *, cleaner, void *);

/*
 * Insert new element at the begging of the list
 */
void push (list_item **, cleaner, void *);

/*
 * Go through list, and launch functions
 */
void clean (list_item *);

#endif // CLEANLIST_H
