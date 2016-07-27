#include <stdlib.h>

#include "my_macros.h"
#include "cleanlist.h"

inline static
pListItem create_item (fpCleaner func, pArgument arg) {
	pListItem where;

	if ((where = malloc (sizeof (ListItem))) == NULL) {
		ERROR ("malloc failed creating list item");
		exit (EXIT_FAILURE);
	}
	where->arg = arg;
	where->func = func;
	where->next = NULL;
	return where;
}

void push_cleaner (pListItem * head, fpCleaner func, pArgument arg) {
	pListItem current = create_item (func, arg);

	current->next = *head;
	*head = current;
}

inline pListItem pop_cleaner (pListItem * head) {
	if ((*head) == NULL) {	// empty list
		return NULL;
	}
	pListItem result = *head;

	(*head) = (*head)->next;
	return result;
}

void cleaning (pListItem * head) {
	pListItem current;

	while ((*head) != NULL) {
		current = pop_cleaner (head);
		current->func (current->arg);
		free (current);
	}
}
