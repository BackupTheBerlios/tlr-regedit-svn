Name: regedit
Version: 0.3
Release: 1
Source0:    http://members.aon.at/gregorburger/%{name}-%{version}.tar.gz
Group:     System Environment/Libraries
Copyright: GPL
Vendor: Gregor Burger <gregor.burger@aon.at>
Packager: Avi Alkalay <avi@unix.sh>
URL: http://www.livejournal.com/users/gregorburger/
BuildRoot:   %{_tmppath}/%{name}-%{version}-build
BuildRequires: registry-devel
Requires: registry
Summary: A GUI to edit Linux Registry keys

%description
A GUI to edit Linux Registry keys, based on QT

%prep
%setup -n regedit
%build
qmake regedit.pro
make

%install
strip src/regedit
mkdir -p $RPM_BUILD_ROOT/usr/bin
cp src/regedit $RPM_BUILD_ROOT/usr/bin
find icons | grep -v .svn | grep \.png | cpio -pdvm $RPM_BUILD_ROOT/usr/share/lib/regedit
mkdir -p $RPM_BUILD_ROOT/usr/share/applications/
cp regedit.desktop $RPM_BUILD_ROOT/usr/share/applications/


%clean
rm -rf $RPM_BUILD_ROOT/*
rm -rf $RPM_BUILD_DIR/regedit




%post
# Set the icons directory for myself
rg set system/sw/regedit/gui/iconDir "/usr/share/lib/regedit/icons"


%files
%defattr(-,root,root,0755)
/usr/bin/*
/usr/share/lib/regedit
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
