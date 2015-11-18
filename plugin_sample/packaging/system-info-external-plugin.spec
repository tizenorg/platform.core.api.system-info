Name:		system-info-external-plugin
Summary:	External plugin for System information library
Version:	0.0.1
Release:	1
Group:		System/Libraries
License:	Apache License, Version 2.0
Source0:	%{name}-%{version}.tar.gz
Source1001:	%{name}.manifest
BuildRequires:	cmake
BuildRequires:	pkgconfig(dlog)
BuildRequires:	pkgconfig(capi-system-info-plugin)

Requires(post):	/sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description

%prep
%setup -q
cp %{SOURCE1001} .

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}

%make_install
mkdir -p %{buildroot}/usr/share/license
cp -f LICENSE %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
/usr/share/license/%{name}
%{_libdir}/libsystem-info-external-plugin.so
