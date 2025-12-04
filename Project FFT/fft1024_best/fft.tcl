open_component fft.comp -reset
add_files [list fft.cpp]
add_files -tb [list fft_test.cpp]
set_top fft
puts "Running: set_top fft"
set_part xc7z020-clg400-1
puts "Running: set_part xc7z020-clg400-1"
create_clock -period 10
csynth_design

exit