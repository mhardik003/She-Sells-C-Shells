main:
	gcc -g global_vars.c init.c main.c prompt.c input_handler.c functions/background.c function_handler.c functions/pastevents.c functions/peek.c functions/proclore.c functions/seek.c functions/syscalls.c functions/warp.c functions/redirection.c functions/iMan.c functions/pipe.c functions/neonate.c functions/activities.c functions/signals.c functions/fg_bg.c

clean:
	rm a.out

run:
	gcc -g global_vars.c init.c main.c prompt.c input_handler.c functions/background.c function_handler.c functions/pastevents.c functions/peek.c functions/proclore.c functions/seek.c functions/syscalls.c functions/warp.c functions/redirection.c functions/iMan.c functions/pipe.c functions/neonate.c functions/activities.c functions/signals.c functions/fg_bg.c
	./a.outa