echo "Build Static-Debug Lib and Testing Programs:"
make -f Makefile-Static-Debug uninstall clean core install 
make -f Makefile-Static-Debug clean
make -f Makefile-Static-Debug-FuncTest uninstall clean functest install 
make -f Makefile-Static-Debug-FuncTest clean
make -f Makefile-Static-Debug-PerfTest uninstall perftest install
make -f Makefile-Static-Debug-PerfTest clean

echo "Build Static-Release Lib and Testing Programs:"
make -f Makefile-Static-Release uninstall clean core install
make -f Makefile-Static-Release clean
make -f Makefile-Static-Release-FuncTest uninstall clean functest install
make -f Makefile-Static-Release-FuncTest clean
make -f Makefile-Static-Release-PerfTest uninstall clean perftest install
make -f Makefile-Static-Release-PerfTest clean
