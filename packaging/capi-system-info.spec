#sbs-git:slp/api/system-info capi-system-info 0.1.0 63d15bafa590ee9de869c8a8ade712e06828e5c3
Name:       capi-system-info
Summary:    A System Information library in SLP C API
Version: 0.1.9
Release:    1
Group:      System/Libraries
License:    Apache License, Version 2.0 and IEFT RFC Collection
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:	pkgconfig(capi-media-sound-manager)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xi)
BuildRequires:	pkgconfig(xrandr)
BuildRequires:  pkgconfig(tapi)
BuildRequires:	pkgconfig(haptic)
BuildRequires:	pkgconfig(xproto)
BuildRequires:	pkgconfig(openssl)
BuildRequires:	pkgconfig(nfc)
BuildRequires:	pkgconfig(location)
BuildRequires:	pkgconfig(bluetooth-api)
BuildRequires:	pkgconfig(mm-radio)
BuildRequires:	pkgconfig(sensor)
BuildRequires:	pkgconfig(gles11)
BuildRequires:  pkgconfig(libxml-2.0)

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A System Information library in SLP C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel


%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}

mkdir -p %{buildroot}/usr/share/license
cp -f LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}
cat LICENSE.RFC4122 >> %{buildroot}/usr/share/license/%{name}

mkdir -p %{buildroot}/etc
cp -f script/make_info_file.sh %{buildroot}/etc/make_info_file.sh

mkdir -p %{buildroot}/etc/config
cp -f configuration/sys-info.xml %{buildroot}/etc/config/sys-info.xml

%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/libcapi-system-info.so.*
/usr/share/license/%{name}
%attr(0744,root,-) /etc/make_info_file.sh
/etc/config/sys-info.xml
%manifest system-info.manifest

%files devel
%{_includedir}/system/system_info.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
