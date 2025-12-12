open_component bnn.comp -reset
add_files [list bnn.cpp bnn.h golden.h]
add_files -tb [list bnn_test.cpp]
set_top bnn
puts "Running: set_top bnn"
set_part xc7z020-clg400-1
puts "Running: set_part xc7z020-clg400-1"
create_clock -period 10
csynth_design

exit