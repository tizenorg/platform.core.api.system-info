%bcond_with x
%bcond_with wayland
Name:           capi-system-info
Version:        0.2.0
Release:        0
License:        Apache-2.0
Summary:        A System Information library in SLP C API
Group:          System/API
Source0:        %{name}-%{version}.tar.gz
Source1001:     capi-system-info.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(iniparser)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(vconf)
%if %{with wayland}
BuildRequires:  pkgconfig(ecore-wayland)
%endif
%if %{with x}
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xi)
BuildRequires:  pkgconfig(xrandr)
%endif

%description
A System Information library in SLP C API

%package devel
Summary:        A System Information library in SLP C API (Development)
Group:          Development/System
Requires:       %{name} = %{version}

%description devel
%devel_desc

%prep
%setup -q
cp %{SOURCE1001} .

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%if !%{with x} && %{with wayland}
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER} -DENABLE_WAYLAND=TRUE
%else
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
%endif

make %{?_smp_mflags}

%install
%make_install
mkdir -p %{buildroot}/etc
cp -f script/make_info_file.sh %{buildroot}%{_sysconfdir}/make_info_file.sh

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%license LICENSE.APLv2
%{_libdir}/libcapi-system-info.so.*
%attr(0744,root,-) %{_sysconfdir}/make_info_file.sh
%manifest system-info.manifest

%files devel
%manifest %{name}.manifest
%{_includedir}/system/system_info.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
