#include <stdio.h>
#include <stdlib.h>

#include "cleanlist.h"

list_item *create_item (void) {
	list_item *where = malloc (sizeof (list_item));

	if (where == NULL) {
		fprintf (stderr, "malloc failed creating list item\n");
		exit (1);
	}
	where->arg = NULL;
	where->func = NULL;
	where->next = NULL;
	return where;
}

void insert (list_item * head, cleaner func, void *arg) {
	if (head == NULL) {
		head = create_item ();
		head->arg = arg;
		head->func = func;
	} else {
		list_item *current = head;

		while (current->next != NULL) {
			current = current->next;
		}
		current->next = create_item ();
		current->next->arg = arg;
		current->next->func = func;
	}
}

void push (list_item ** head, cleaner func, void *arg) {
	list_item *current = create_item ();

	current->arg = arg;
	current->func = func;
	current->next = *head;
	*head = current;
}

void clean (list_item * head) {
	while (head != NULL) {
		head->func (head->arg);
		head = head->next;
	}
}
