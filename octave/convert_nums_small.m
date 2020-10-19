clear all;

% Digit sizes in 8x8 blocks
block_width = 4
block_height = 6
num_width = block_width*8
num_height = block_height*8

% Start and end locations for each digit (1 block border, 1 block between numbers)
num_start_x = (1 + (0:4)*(block_width+1))*8 + 1;
num_start_x = [num_start_x num_start_x];
num_end_x = num_start_x + num_width - 1;

num_start_y = ones(1, 5)*9;
num_start_y = [num_start_y (num_start_y + num_height + 8)];
num_end_y = num_start_y + num_height - 1;

img = imread("numbers_small.png");
img_data = (img(:,:,1) > 5); % Threshold the image red channel (ignores the outline and grid)

% Separate the numbers and put them all into a height*width*number matrix
num_data = logical(zeros(num_height, num_width, 10));
for n = 1:10
  num_data(:,:,n) = img_data(num_start_y(n):num_end_y(n), num_start_x(n):num_end_x(n));
endfor

% Rotate the data CCW so it matches the epaper RAM direction
num_data_rot = rot90(num_data);

% Output data needs the horizontal direction packed into bytes
out = zeros(num_width, num_height/8, 10);

for n = 1:10 % For each digit
  n
  for j = 1:num_width % For each row (after rotating)
    for i = 1:num_height % Set the appropriate bits in each byte of output data
      byte = floor((i-1)/8) + 1;
      bit = (7 - mod(i-1, 8)) + 1;
      out(j, byte, n) = bitset(out(j, byte, n), bit, num_data_rot(j, i, n));
    endfor
  endfor
endfor

% display for debug
j=1
n=7
b = uint8(num_data_rot(:,:,n)*255);
imshow(b);
grid on;
out(1,:, n)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Save to .h file
filename = "data_nums_small.h";
fid = fopen(filename, "w");

fprintf(fid, "#define NUM_SML_XSIZE \t%d\n", num_height/8);
fprintf(fid, "#define NUM_SML_YSIZE \t%d\n", num_width);
fprintf(fid, "\n");

for n = 1:10
  fprintf(fid, "// Small digit %d (%dx%d)\n", n-1, num_width, num_height);
  fprintf(fid, "const unsigned char num_sml_%d[] PROGMEM = {\n", n-1);
  for j = 1:num_width
    for i = 1:num_height/8
      if (i == 1) fprintf(fid, "\t"); endif;
      fprintf(fid, "0x%02X", out(j, i, n));
      if (i == num_height/8) % end of row
        if (j == num_width) % last data in number
          fprintf(fid, "\n};\n");
        else
          fprintf(fid, ",\n");
        endif
      else
        fprintf(fid, ", ");
      endif;
    endfor
  endfor

  if (n != 10) % not last number
    fprintf(fid, "\n");
  endif

endfor

fclose (fid);
