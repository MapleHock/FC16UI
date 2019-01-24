import os
import Image

UNIT_SIZE = 32 #in pt
NUM_PER_ROW = 7
TARGET_WIDTH = UNIT_SIZE * NUM_PER_ROW
TARGET_HEIGHT = 5 * UNIT_SIZE

path = "E:/programming/tile_map_test/new_map_csv_test/tileset/workspace/"
# images_name = []
# for root, dirs, files in os.walk(path):
#     for f in files :
#         images_name.append(f)

#need a order user const images_name
images_name = []
type_list = ["LightInfantry", "LightArcher", "LightKnight", "Mangonel", "HeavyInfantry", "HeavyArcher", "HeavyKnight"]
for i in range(4):
    for soldier in type_list:
        images_name.append(soldier + str(i) + ".png")
        print(soldier + str(i) + ".png")
  

left = 0
right = UNIT_SIZE 
top = UNIT_SIZE # 0 for unblank one
bottom = UNIT_SIZE * 2 # UNIT_SIZE for unblank one
target = Image.new('RGBA', (TARGET_WIDTH, TARGET_HEIGHT), (0, 0, 0, 1)) 

 #add the first blank row
# for i in range(NUM_PER_ROW):
#     image = images[i]
#     target.paste(image, (left, 0, right, UNIT_SIZE))
#     left += UNIT_SIZE
#     right += UNIT_SIZE


for i in range(len(images_name)/NUM_PER_ROW):
    left = 0
    right = UNIT_SIZE
    for j in range(NUM_PER_ROW):
        image = Image.open(path + images_name[i * NUM_PER_ROW + j])
        target.paste(image, (left, top, right, bottom))
        left += UNIT_SIZE
        right += UNIT_SIZE
    top += UNIT_SIZE
    bottom += UNIT_SIZE

target.save(path + "merge.png")       
