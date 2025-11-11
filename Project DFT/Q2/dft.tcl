open_component dft.comp -reset
add_files [list dft.cpp]
add_files -tb [list dft-top.cpp]
set_top dft
puts "Running: set_top dft"
set_part xc7z020-clg400-1
puts "Running: set_part xc7z020-clg400-1"
create_clock -period 10
csynth_design

exit