clear all;

% Digit sizes for MEM,1,2,3 digits, LOWBAT
width = [64, 14, 14, 14, 120];
height = [24, 24, 24, 24, 24];

% Start and end locations for each graphic (+1 for octave numbering)
start_x = [8, 80, 96, 112, 8] +1;
start_y = [8, 8, 8, 8, 40] +1;

end_x = start_x + width - 1;
end_y = start_y + height - 1;

img = imread("misc.png");
img_data = (img(:,:,1) > 5); % Threshold the image red channel (ignores the outline and grid)

% Get data from the image
data_MEM = img_data(start_y(1):end_y(1), start_x(1):end_x(1));
data_1 = img_data(start_y(2):end_y(2), start_x(2):end_x(2));
data_2 = img_data(start_y(3):end_y(3), start_x(3):end_x(3));
data_3 = img_data(start_y(4):end_y(4), start_x(4):end_x(4));
data_BAT = img_data(start_y(5):end_y(5), start_x(5):end_x(5));

% Rotate the data CCW so it matches the epaper RAM direction
data_MEM_rot = rot90(data_MEM);
data_1_rot = rot90(data_1);
data_2_rot = rot90(data_2);
data_3_rot = rot90(data_3);
data_BAT_rot = rot90(data_BAT);

% Output data needs the horizontal direction packed into bytes
out_MEM = zeros(width(1), height(1)/8);
out_1 = zeros(width(2), height(2)/8);
out_2 = zeros(width(3), height(3)/8);
out_3 = zeros(width(4), height(4)/8);
out_BAT = zeros(width(5), height(5)/8);

% Convert to bytes (MEM)
for j = 1:width(1) % For each row (after rotating)
  for i = 1:height(1) % Set the appropriate bits in each byte of output data
    byte = floor((i-1)/8) + 1;
    bit = (7 - mod(i-1, 8)) + 1;
    out_MEM(j, byte) = bitset(out_MEM(j, byte), bit, data_MEM_rot(j, i));
  endfor
endfor

% Convert to bytes (digits)
for j = 1:width(2) % For each row (after rotating)
  for i = 1:height(2) % Set the appropriate bits in each byte of output data
    byte = floor((i-1)/8) + 1;
    bit = (7 - mod(i-1, 8)) + 1;
    out_1(j, byte) = bitset(out_1(j, byte), bit, data_1_rot(j, i));
    out_2(j, byte) = bitset(out_2(j, byte), bit, data_2_rot(j, i));
    out_3(j, byte) = bitset(out_3(j, byte), bit, data_3_rot(j, i));
  endfor
endfor

% Convert to bytes (BAT)
for j = 1:width(5) % For each row (after rotating)
  for i = 1:height(5) % Set the appropriate bits in each byte of output data
    byte = floor((i-1)/8) + 1;
    bit = (7 - mod(i-1, 8)) + 1;
    out_BAT(j, byte) = bitset(out_BAT(j, byte), bit, data_BAT_rot(j, i));
  endfor
endfor

% display for debug
b = uint8(data_BAT_rot(:,:)*255);
imshow(b);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Save to .h file
filename = "data_misc.h";
fid = fopen(filename, "w");

fprintf(fid, "#define TXT_MEM_XSIZE \t%d\n", height(1)/8);
fprintf(fid, "#define TXT_MEM_YSIZE \t%d\n", width(1));
fprintf(fid, "#define NUM_MEM_XSIZE \t%d\n", height(2)/8);
fprintf(fid, "#define NUM_MEM_YSIZE \t%d\n", width(2));
fprintf(fid, "#define TXT_BAT_XSIZE \t%d\n", height(5)/8);
fprintf(fid, "#define TXT_BAT_YSIZE \t%d\n", width(5));
fprintf(fid, "\n");

fprintf(fid, "// MEM text (inverted) (%dx%d)\n", width(1), height(1));
fprintf(fid, "const unsigned char txt_mem[] PROGMEM = {\n");
for j = 1:width(1)
  for i = 1:height(1)/8
    if (i == 1) fprintf(fid, "\t"); endif;
    fprintf(fid, "0x%02X", out_MEM(j, i));
    if (i == height(1)/8) % end of row
      if (j == width(1)) % last data in number
        fprintf(fid, "\n};\n");
      else
        fprintf(fid, ",\n");
      endif
    else
      fprintf(fid, ", ");
    endif;
  endfor
endfor
fprintf(fid, "\n");

fprintf(fid, "// digit 1 for MEM (inverted) (%dx%d)\n", width(2), height(2));
fprintf(fid, "const unsigned char num_mem_1[] PROGMEM = {\n");
for j = 1:width(2)
  for i = 1:height(2)/8
    if (i == 1) fprintf(fid, "\t"); endif;
    fprintf(fid, "0x%02X", out_1(j, i));
    if (i == height(2)/8) % end of row
      if (j == width(2)) % last data in number
        fprintf(fid, "\n};\n");
      else
        fprintf(fid, ",\n");
      endif
    else
      fprintf(fid, ", ");
    endif;
  endfor
endfor
fprintf(fid, "\n");

fprintf(fid, "// digit 2 for MEM (inverted) (%dx%d)\n", width(3), height(3));
fprintf(fid, "const unsigned char num_mem_2[] PROGMEM = {\n");
for j = 1:width(3)
  for i = 1:height(3)/8
    if (i == 1) fprintf(fid, "\t"); endif;
    fprintf(fid, "0x%02X", out_2(j, i));
    if (i == height(3)/8) % end of row
      if (j == width(3)) % last data in number
        fprintf(fid, "\n};\n");
      else
        fprintf(fid, ",\n");
      endif
    else
      fprintf(fid, ", ");
    endif;
  endfor
endfor
fprintf(fid, "\n");

fprintf(fid, "// digit 3 for MEM (inverted) (%dx%d)\n", width(4), height(4));
fprintf(fid, "const unsigned char num_mem_3[] PROGMEM = {\n");
for j = 1:width(4)
  for i = 1:height(4)/8
    if (i == 1) fprintf(fid, "\t"); endif;
    fprintf(fid, "0x%02X", out_3(j, i));
    if (i == height(4)/8) % end of row
      if (j == width(4)) % last data in number
        fprintf(fid, "\n};\n");
      else
        fprintf(fid, ",\n");
      endif
    else
      fprintf(fid, ", ");
    endif;
  endfor
endfor
fprintf(fid, "\n");

fprintf(fid, "// LOWBAT text (inverted) (%dx%d)\n", width(5), height(5));
fprintf(fid, "const unsigned char bat_mem[] PROGMEM = {\n");
for j = 1:width(5)
  for i = 1:height(5)/8
    if (i == 1) fprintf(fid, "\t"); endif;
    fprintf(fid, "0x%02X", out_BAT(j, i));
    if (i == height(5)/8) % end of row
      if (j == width(5)) % last data in number
        fprintf(fid, "\n};\n");
      else
        fprintf(fid, ",\n");
      endif
    else
      fprintf(fid, ", ");
    endif;
  endfor
endfor

fclose (fid);
