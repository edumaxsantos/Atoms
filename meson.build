project('chemistry', 'cpp',
        version: '1.0.0',
        default_options: ['cpp_std=c++23', 'warning_level=3'])

subdir('src')

raylib_dep = dependency('raylib', required: true)
nlohmann_dep = dependency('nlohmann_json')
executable('chemistry', src_files, 
        dependencies: [raylib_dep, nlohmann_dep]
)