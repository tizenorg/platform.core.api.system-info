Name:           capi-system-info-n4
Version:        0.2.0
Release:        0
License:        Apache-2.0
Summary:        A System Information library in Core API
Group:          System/API
Source0:        %{name}-%{version}.tar.gz
Source1001:     %{name}.manifest
Source2001:     tizenid.service
BuildRequires:  cmake
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(iniparser)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:	model-config-n4

%description


%package devel
Summary:  A System Information library in Core API (Development)
Group:    Development/System
Requires: %{name} = %{version}-%{release}

%description devel


%prep
%setup -q
cp %{SOURCE1001} .

%define config_file_path /etc/config/model-config.xml
%define info_file_path /etc/info.ini
%define os_release_file_path /etc/os-release
%define serial_path /csa/imei/serialno.dat
%define tizen_id_path /etc/tizenid
%define use_tizen_prop 1

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`

%cmake . -DCONFIG_FILE_PATH=%{config_file_path} \
		 -DINFO_FILE_PATH=%{info_file_path} \
		 -DOS_RELEASE_FILE_PATH=%{os_release_file_path} \
		 -DSERIAL_PATH=%{serial_path} \
		 -DMAJORVER=${MAJORVER} \
		 -DFULLVER=%{version} \
		 -DTIZEN_ID_PATH=%{tizen_id_path} \
		 -DUSE_TIZEN_PROP=%{use_tizen_prop} \
		 -DTIZEN_PROP_PATH=.

%__make %{?_smp_mflags}

%install
%make_install
mkdir -p %{buildroot}/etc
cp -f script/make_info_file.sh %{buildroot}/etc/make_info_file.sh

cp %{buildroot}/usr/include/system/system_info_tizen_prop.h %{buildroot}/
cp script/tizen_prop.sh %{buildroot}/
%{buildroot}/tizen_prop.sh %{buildroot}
mv %{buildroot}/libsystem-info-tizen-prop.so %{buildroot}/%{_libdir}/
rm %{buildroot}/system_info_tizen_prop.h
rm %{buildroot}/tizen_prop.sh

%install_service multi-user.target.wants tizenid.service

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%license LICENSE
%{_libdir}/libcapi-system-info.so.*
%{_libdir}/libsystem-info-tizen-prop.so
%attr(0744,root,-) /etc/make_info_file.sh

#tizenid
%{_bindir}/tizen_id

%{_unitdir}/tizenid.service
%{_unitdir}/multi-user.target.wants/tizenid.service

%files devel
%manifest %{name}.manifest
%{_includedir}/system/system_info.h
%{_includedir}/system/system_info_type.h
%{_includedir}/system/system_info_tizen_prop.h
%{_includedir}/plugin/system_info_intf.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-info.so
