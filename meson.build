project(
	'BurningGround',
	['c', 'cpp'],
	version: '1.0',
	default_options: [
		'c_std=c99',
		'cpp_std=c++17',
		'build.c_std=c99',
		'build.cpp_std=c++17',
	],
)

is_static = false

app_exe = get_option('app_exe')
project_src_files = []
project_inc = include_directories([ 'include' ])
project_c_args = ['-Wall']
project_cpp_args = []
project_link_args = []
project_deps = []

cc = meson.get_compiler('c')

curses_dep = dependency('ncurses', required: false, disabler: true, static: is_static)
if not curses_dep.found()
	# Searching for pdcurses
	curses_root = get_option('curses_root')
	curses_lib = cc.find_library('curses', dirs : curses_root, required : false, disabler: true)
	curses_dep = declare_dependency(include_directories: curses_root, dependencies: curses_lib, static: false)
endif

m_dep = cc.find_library('m', static: is_static)

project_deps = [curses_dep, m_dep]

subdir('src')

executable(
	app_exe,
	sources: project_src_files,
	include_directories: project_inc,
	c_args: project_c_args,
	cpp_args: project_cpp_args,
	link_args: project_link_args,
	dependencies: project_deps,
)
