CC       = @gcc
CFLAGS   = -ansi -pedantic-errors -Wall -Wextra -g -fsanitize=address
HPATHS 	 = -iquote ../uid -iquote ../sorted_dl_list -iquote ../doubly_linked_list
FILES    = st_scheduler_main.c st_scheduler.c task.c ../uid/uid.c ../sorted_dl_list/sorted_dl_list.c ../doubly_linked_list/dl_list.c

scheular:
	$(CC) $(CFLAGS) $(HPATHS) $(FILES)
