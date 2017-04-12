# https://opencppcoverage.codeplex.com/wikipage?title=Command-Line%20Reference

${codeCover}    = 'C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe'

${solution_dir} = 'C:\Users\R-J Wolthuis\git\cpp\Sudoku'

${mode}         = 'Debug' # Debug / Release


#${module_incl}  = * # default
${module_incl}  = "SudokuTests.exe"
${module_incl_1}  = "Console"

${source_incl}  = "${solution_dir}\*"
${work_dir}     = "${solution_dir}\SudokuTests\Coverage"

${export}       = "html:${work_dir}\${mode}"
${export_1}     = "binary:${work_dir}\${mode}\Console.cov"

${execute}      = "${solution_dir}\${mode}\SudokuTests.exe"
${execute_1}      = "${solution_dir}\${mode}\Console.exe"

cd $work_dir
& $codeCover -q --modules $module_incl_1 --sources $source_incl --export_type $export_1 --working_dir $work_dir -- $execute_1

#& $codeCover -q --modules $module_incl --sources $source_incl --export_type $export --cover_children --no_aggregate_by_file -- $execute
#& $codeCover --modules $module_incl --sources $source_incl --export_type $export --working_dir $work_dir -- $execute
& $codeCover -q --modules $module_incl --sources $source_incl --export_type $export --input_coverage=Debug\Console.cov --working_dir $work_dir -- $execute

