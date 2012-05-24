clean:
	find . -name "*~" -exec rm '{}' \;
	find . -name "*.orig" -exec rm '{}' \;
indent:
	find . -name "*.h" -exec astyle '{}' \;
	find . -name "*.cpp" -exec astyle '{}' \;
