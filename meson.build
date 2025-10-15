project(
	'pwgtk', 
	['c'],
	version: '0.0.1',
	default_options : ['c_std=c11'],
	license: 'GPL-3.0-or-later',
)

project_description = 'Miscelaneous gtk4 widgets'

#
# Versioning
#
pkg_version = meson.project_version()
version_array = pkg_version.split('.')
pkg_major_version = version_array[0].to_int()
pkg_minor_version = version_array[1].to_int()
pkg_micro_version = version_array[2].to_int()

libversion = '@0@.@1@'.format(pkg_major_version,pkg_minor_version)

#
# Compilers
#
#cxx = meson.get_compiler('cpp')
cc = meson.get_compiler('c')
enable_debug = get_option('debug') or get_option('buildtype').contains('debug')

compiler_flags_common = [
	]

if enable_debug
  compiler_flags_common += [
    '-ggdb3',
	'-DDEBUG=1'
  ]
else
  compiler_flags_common += [
	'-DNDEBUG=1'
  ]
endif

global_cflags = cc.get_supported_arguments(compiler_flags_common)

add_project_arguments(global_cflags, language: 'c')

#
# Configuration
#
conf_data = configuration_data()
conf_data.set_quoted('PACKAGE_NAME', meson.project_name())
conf_data.set_quoted('PACKAGE_VERSION', meson.project_version())
conf_data.set_quoted('PACKAGE_DESCRIPTION', project_description)

if cc.compiles('#include <unistd.h>', name : 'unistd.h')
  conf_data.set('HAVE_UNISTD_H', 1)
endif

configure_file(output : 
	'config.h', 
	configuration : conf_data
)

includes_dir = include_directories('../src/include')

dependencies = [
	dependency('gtk4'),
]

sources = [
	'src/widget/progress.c', 
]

if host_machine.system() == 'windows'

  # https://mesonbuild.com/Windows-module.html
  #windows = import('windows')
  #resources = windows.compile_resources(
  #  configure_file(
  #    input : 'src/library/os/windows/resources.rc.in',
  #    output : 'resources.rc',
  #    configuration : app_conf
  #  )
  #)

  dynamic = shared_library(
    meson.project_name(), 
    sources,
    install: true,
    gnu_symbol_visibility: 'hidden',
    dependencies: dependencies,
    include_directories: includes_dir,
  )

  static_library = declare_dependency(
    link_with : static_library(
		meson.project_name(), 
		sources,
		c_args: [ '-DSTATIC_LIBRARY' ],
		install: true,
		dependencies: dependencies,
		include_directories: includes_dir
	),
	include_directories : includes_dir,
  ) 


elif host_machine.system() == 'darwin'

  dynamic = shared_library(
	meson.project_name(), 
	sources,
    install: true,
    version : libversion,
    soversion : libversion,
    gnu_symbol_visibility: 'hidden',
    dependencies: dependencies,
    include_directories: includes_dir
  )

  static_library = declare_dependency(
      link_with : static_library(
		meson.project_name(), 
		sources,
		pic: true,
		c_args: [ '-DSTATIC_LIBRARY' ],
		install: true,
		dependencies: dependencies,
		include_directories: includes_dir
	),
    include_directories : includes_dir,
  ) 

else

  dynamic = shared_library(
	meson.project_name(), 
	sources,
    install: true,
    version : libversion,
    soversion : libversion,
    c_args: [ '-ffat-lto-objects', '-fPIC' ],
    gnu_symbol_visibility: 'hidden',
	dependencies: dependencies,
    include_directories: includes_dir
  )

  static_library = declare_dependency(
    link_with : static_library(
		meson.project_name(), 
		sources,
		c_args: [ '-DSTATIC_LIBRARY', '-fno-lto', '-fPIC' ],
		install: true,
		dependencies: dependencies,
		include_directories: includes_dir
    ),
	include_directories : includes_dir,
  ) 

endif

dynamic_library = declare_dependency(
  link_with : dynamic,
  include_directories : includes_dir,
) 

#executable(
#  meson.project_name(),
#  config_src + [ 'src/testprogram/testprogram.c' ],
#  install: false,
#  link_with : [ dynamic ],
#  dependencies: deps,
#  include_directories: includes_dir
#)
