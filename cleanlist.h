#ifndef CLEANLIST_H
#  define CLEANLIST_H

typedef void (*fpCleaner) (void *);
typedef void *pArgument;
typedef struct sListItem ListItem;
typedef ListItem *pListItem;
struct sListItem {
	fpCleaner func;
	void *arg;
	pListItem next;
};

/*
 * Insert new element at the beginning of the list
 */
void push_cleaner (pListItem *, fpCleaner, pArgument);

/**
 * Remove element from the beginning of the list
 */
pListItem pop_cleaner (pListItem *);

/*
 * Go through list, and launch functions
 */
void cleaning (pListItem *);

#endif				// CLEANLIST_H
