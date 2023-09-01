src = run_command('find', './src', '-name', '*.c', check : true).stdout().strip().split('\n')

stdout_logger_exe = executable(
    'stdout_logger',
    src,
    dependencies : [rcpr, vpr, vccert, vccrypt, vcservice_dep]
)
