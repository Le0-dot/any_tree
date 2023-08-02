project('demo', 'cpp',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

run_command('mkdir', '-p', 'subprojects/any_tree', check: true)
run_command('cp', '-r', '../include/', '../meson.build', './subprojects/any_tree', check: true)

any_tree_proj = subproject('any_tree')
any_tree_dep = any_tree_proj.get_variable('any_tree_dep')

executable('demo', 'demo.cpp',
  dependencies: any_tree_dep,
  install: true)
