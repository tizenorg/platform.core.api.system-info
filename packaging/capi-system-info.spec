#sbs-git:slp/api/system-info capi-system-info 0.1.0 63d15bafa590ee9de869c8a8ade712e06828e5c3
Name:       capi-system-info
Summary:    A System Information library in SLP C API
Version: 0.1.5
Release:    0
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:	pkgconfig(capi-network-wifi)
BuildRequires:	pkgconfig(capi-media-sound-manager)
BuildRequires:	pkgconfig(capi-uix-stt)
BuildRequires:	pkgconfig(wifi-direct)
BuildRequires:  pkgconfig(iniparser)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xi)
BuildRequires:	pkgconfig(xrandr)
BuildRequires:  pkgconfig(tapi)
BuildRequires:	pkgconfig(devman)
BuildRequires:	pkgconfig(xproto)
BuildRequires:	pkgconfig(openssl)
BuildRequires:	pkgconfig(nfc)
BuildRequires:	pkgconfig(location)
BuildRequires:	pkgconfig(bluetooth-api)
BuildRequires:	pkgconfig(mm-radio)
BuildRequires:	pkgconfig(sensor)
%ifarch %{ix86}
BuildRequires:	simulator-opengl-devel
%else
BuildRequires:	pkgconfig(gles11)
%endif

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
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/libcapi-system-info.so.*

%files devel
%{_includedir}/system/system_info.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
