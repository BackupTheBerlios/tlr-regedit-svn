Name: kdbe
Version: 0.3
Release: 1
Source0:    http://members.aon.at/gregorburger/%{name}-%{version}.tar.gz
Group:     System Environment/Libraries
Copyright: GPL
Vendor: Gregor Burger <gregor.burger@aon.at>
Packager: Avi Alkalay <avi@unix.sh>
URL: http://www.livejournal.com/users/gregorburger/
BuildRoot:   %{_tmppath}/%{name}-%{version}-build
BuildRequires: elektra-devel
Requires: elektra
Summary: A GUI to edit the Elektra Key Database

%description
A GUI to edit Elektra Key Database, based on QT

%prep
%setup -n kdbe
%build
qmake kdbe.pro
make

%install
strip src/kdbe
mkdir -p $RPM_BUILD_ROOT/usr/bin
cp src/regedit $RPM_BUILD_ROOT/usr/bin
find icons | grep -v .svn | grep \.png | cpio -pdvm $RPM_BUILD_ROOT/usr/share/lib/kdbe
mkdir -p $RPM_BUILD_ROOT/usr/share/applications/
cp kdbe.desktop $RPM_BUILD_ROOT/usr/share/applications/


%clean
rm -rf $RPM_BUILD_ROOT/*
rm -rf $RPM_BUILD_DIR/kdbe




%post
# Set the icons directory for myself
rg set system/sw/kdbe/gui/iconDir "/usr/share/lib/kdbe/icons"


%files
%defattr(-,root,root,0755)
/usr/bin/*
/usr/share/lib/kdbe
/usr/share/applications/*

%changelog
* Tue Aug 10 2004 Avi Alkalay <avi@unix.sh> 0.3-1
- Updated version

* Sat Jul 24 2004 Avi Alkalay <avi@unix.sh> 0.2.1-2
- Added desktop file, to get an icon in the apps menu, under System Tools
- Improvements to the spec file

* Tue Jul 20 2004 Avi Alkalay <avi@unix.sh> 0.2.1-1
- Updated to 0.2.1 and a qmake build

* Fri Jul 16 2004 Avi Alkalay <avi@unix.sh> 0.2
- First package with a manual build
