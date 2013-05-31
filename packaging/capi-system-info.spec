Name:       capi-system-info
Summary:    A System Information library in SLP C API
Version: 0.1.14
Release:    0
Group:      System/API
License:    Apache-2.0 and IEFT RFC Collection
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

%description
A System Information library in SLP C API


%package devel
Summary:  A System Information library in SLP C API (Development)
Group:    Development/System
Requires: %{name} = %{version}-%{release}

%description devel
%devel_desc


%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
%make_install
mkdir -p %{buildroot}/etc
cp -f script/make_info_file.sh %{buildroot}/etc/make_info_file.sh

mkdir -p %{buildroot}/etc/config
cp -f configuration/sys-info.xml %{buildroot}/etc/config/sys-info.xml


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%license LICENSE.APLv2 LICENSE.RFC4122 
%{_libdir}/libcapi-system-info.so.*
%attr(0744,root,-) /etc/make_info_file.sh
/etc/config/sys-info.xml
%manifest system-info.manifest

%files devel
%{_includedir}/system/system_info.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
