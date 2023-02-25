local rect = Rectangle(10, 20, 110, 220)

print(
    rect.leftTop.x, rect.leftTop.y,
    rect.rightBottom.x, rect.rightBottom.y
)

print(rect:width(), rect:height())
print(rect:area())