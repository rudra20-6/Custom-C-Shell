make:
	gcc FinalShell/mainrunner.c FinalShell/reveal.c FinalShell/parser.c FinalShell/hop.c FinalShell/systemCommands.c FinalShell/log.c FinalShell/proclore.c FinalShell/seek.c FinalShell/redirection.c FinalShell/piping.c FinalShell/activities.c FinalShell/ping.c FinalShell/fgbg.c FinalShell/neonate.c FinalShell/iMan.c -o shell
	
clean:
	rm ./shell