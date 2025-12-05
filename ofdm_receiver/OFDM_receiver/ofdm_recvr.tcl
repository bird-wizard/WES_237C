open_component ofdm_recvr.comp -reset
add_files [list ofdm_recvr.cpp fft.cpp qpsk_decode.cpp]
add_files -tb [list ofdm_test.cpp]
set_top ofdm_receiver
puts "Running: set_top ofdm_receiver"
set_part xc7z020-clg400-1
puts "Running: set_part xc7z020-clg400-1"
create_clock -period 10
csynth_design

exit