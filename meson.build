project('BurningGround', ['c', 'cpp'])

app_exe = get_option('app_exe')
project_src_files = []
project_inc = include_directories([ 'include' ])
project_c_args = []
project_cpp_args = []
project_link_args = []
project_deps = []

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
