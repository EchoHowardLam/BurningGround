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

is_static = get_option('is_static')

app_exe = get_option('app_exe')
project_src_files = []
project_inc = include_directories([ 'include' ])
project_c_args = []
project_cpp_args = []
project_link_args = ['-lm']
project_deps = []


cc = meson.get_compiler('c')


curses_dep = dependency('ncurses', required: false, disabler: true, static: is_static)
if not curses_dep.found()
	# Searching for pdcurses
	curses_root = get_option('win_curses_root')
	curses_full_root = meson.current_source_dir() + curses_root
	message('Searching ' + curses_full_root)
	curses_lib = cc.find_library('pdcurses', dirs : curses_full_root, required : true, disabler: true)
	curses_dep = declare_dependency(include_directories: '.' + curses_root + '\\include', dependencies: curses_lib) # , static: is_static
endif
project_deps += curses_dep


host_platform = host_machine.system()
if cc.get_id() in [ 'msvc' ]
	if host_platform != 'windows'
		error('this seems fishy')
	endif
elif cc.get_id() in [ 'gcc' ] and host_platform == 'windows'
	if get_option('b_lto')
		warning('mingw does not like static + lto')
	endif
elif host_platform in [ 'darwin' ]
elif host_platform in [ 'android' ]
else
	if host_platform != 'linux'
		warning('host platform may not be linux but we will pretend that it is')
		host_platform = 'linux'
	endif
	#m_dep = cc.find_library('m', static: is_static)
	#project_deps += m_dep
endif


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
