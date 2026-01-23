# -*- coding: utf-8 -*-
with open("mainwindow.ui", "r", encoding="utf-8") as f:
    lines = f.readlines()

print("Searching for the problem...")
print()

# 查找 techThemeButton widget
tech_button_idx = -1
for i, line in enumerate(lines):
    if 'techThemeButton' in line and '<widget' in line:
        tech_button_idx = i
        print(f"Found techThemeButton at line {tech_button_idx+1}")
        break

if tech_button_idx == -1:
    print("ERROR: Could not find techThemeButton widget")
    exit(1)

# 检查前一行是否是 <item>
prev_line = lines[tech_button_idx - 1].strip() if tech_button_idx > 0 else ""
print(f"Previous line: {repr(prev_line)}")

if prev_line == "<item>":
    print("OK: techThemeButton is wrapped in <item> tag")
else:
    print(f"PROBLEM: techThemeButton is NOT wrapped in <item> tag")
    print("\nFixing...")
    
    # 在 techThemeButton 之前添加 <item>
    lines.insert(tech_button_idx, "             <item>\n")
    print(f"Added <item> before line {tech_button_idx+1}")
    
    # 找到 techThemeButton 的 </widget> 行
    tech_widget_end_idx = -1
    for i in range(tech_button_idx, min(tech_button_idx + 20, len(lines))):
        if 'techThemeButton' in lines[i] and '</widget>' in lines[i]:
            tech_widget_end_idx = i
            print(f"Found techThemeButton </widget> at line {tech_widget_end_idx+1}")
            break
    
    if tech_widget_end_idx > 0:
        # 在 </widget> 之后添加 </item>
        lines.insert(tech_widget_end_idx + 1, "             </item>\n")
        print(f"Added </item> after line {tech_widget_end_idx+2}")
    
    # 保存
    with open("mainwindow.ui", "w", encoding="utf-8") as f:
        f.writelines(lines)
    
    print(f"\nFixed! Total lines: {len(lines)}")
    print(f"Added {len(lines) - 1396} lines")
    
    # 验证
    new_item_count = 0
    new_item_close_count = 0
    for line in lines:
        if "<item>" in line:
            new_item_count += 1
        if "</item>" in line:
            new_item_close_count += 1
    
    print(f"\nValidation:")
    print(f"  <item> open: {new_item_count}")
    print(f"  </item> close: {new_item_close_count}")
    print(f"  Balance: {new_item_count - new_item_close_count}")
    
    if new_item_count == new_item_close_count:
        print("\nSUCCESS: Item tags are balanced!")
    else:
        print(f"\nERROR: Item tags still imbalanced by {new_item_close_count - new_item_count}")
