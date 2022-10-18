%global debug_package %{nil}
Name: demo
Version: 1.0
Release:        1%{?dist}
Summary: rpm demo package.

License: GPL
#URL:
#Source0:

#BuildRequires:
#Requires:

%description
The demo for test build rpm package.

%prep
%autosetup


%build
#%%configure
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%post

%preun

%postun

%files
/usr/bin/test

%changelog
* Tue Oct 18 2022 fluidog <fluidog@163.com> 1.0-1
- First build.
