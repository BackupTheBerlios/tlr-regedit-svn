Name: regedit
Version: 0.2
Release: 1
Source:    http://registry.sf.net/%{name}-%{version}.tar.gz
Group:     System Environment/Libraries
Copyright: GPL
Vendor: Gregor Burger <gregor.burger@aon.at>
Packager: Avi Alkalay <avi@unix.sh>
URL: http://registry.sf.net
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
strip regedit
find regedit | cpio -pdvm $RPM_BUILD_ROOT/usr/bin
find icons | grep \.png | cpio -pdvm $RPM_BUILD_ROOT/usr/share/lib/regedit


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

%changelog


