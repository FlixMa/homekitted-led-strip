import cv2 as cv
import numpy as np

video = cv.VideoCapture('./water.mov')
ret, frame = video.read()
ms_per_frame = 1000 // 60


lines = []
max_num_lines = 200

for _ in range(max_num_lines):
    ret, frame = video.read()
    if not ret:
        break

    line = frame[0:1]
    line = cv.resize(line, (150, 1), interpolation=cv.INTER_CUBIC)
    lines.append(line[0].tolist())

    #cv.imshow('video', line)
    #if cv.waitKey(ms_per_frame) != -1:
    #    break

with open('animation_sequence.h', 'w') as file:
    num_frames = len(lines)
    width = len(lines[0])
    channels = len(lines[0][0])
    file.write('''#ifndef _ANIMATION_SEQUENCE_H_
#define _ANIMATION_SEQUENCE_H_

#define ANIMATION_SEQUENCE_NUM_FRAMES ({})
#define ANIMATION_SEQUENCE_NUM_PIXEL_PER_FRAME ({})
#define ANIMATION_SEQUENCE_NUM_CHANNELS_PER_PIXEL ({})

uint8_t animation_sequence[ANIMATION_SEQUENCE_NUM_FRAMES][ANIMATION_SEQUENCE_NUM_PIXEL_PER_FRAME][ANIMATION_SEQUENCE_NUM_CHANNELS_PER_PIXEL]'''\
    .format(num_frames, width, channels))
    file.write(' = {\n')

    for i, line in enumerate(lines):
        file.write(str(line).replace('[', '{').replace(']', '}'))
        if i + 1 < num_frames:
            file.write(',\n')
    file.write('''};
#endif //_ANIMATION_SEQUENCE_H_
    ''')

