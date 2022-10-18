rpm-pkg:
	rpmbuild --noprep --build-in-place --define="_topdir $$PWD/rpmbuild" -bb demo.spec
tar-pkg:
	tar -czf demo.tar.gz --exclude='rpmbuild/*' *
