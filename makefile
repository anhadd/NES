# Used in order to allow make commands from the main project directory.
# Only executes the same make command from inside the src directory.
all: 
	+$(MAKE) -C src

clean:
	+$(MAKE) clean -C src
