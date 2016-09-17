include config.mk


default: rmdirs all done

all: mkdirs
	@make -s -C src
	@make -s -C tests

debug: mkdirs
	make -C src
	@make -C tests

mkdirs: rmdirs
	@mkdir bin
	@mkdir obj
	@mkdir lib

rmdirs:
	@rm -rf bin
	@rm -rf obj
	@rm -rf lib

clean: rmdirs
	@echo "cleaning ..."
	@echo "done! :)"

done:
	@echo "done! :)"
