#import pandas as pd
import sys
import statistics as stats

if len(sys.argv) < 2:
    print("Filename argumentum needed")
    sys.exit(1)

filename = sys.argv[1]
print(f"Filename: {filename}")

comments = []
data_lines = []

# --- 1: fájl beolvasás ---
with open(filename) as f:
    for line in f:
        line = line.strip()
        if not line:
            continue
        if line.startswith("#"):
            comments.append(line)
        else:
            data_lines.append(line.split("\t"))

# --- 2: szétválasztás ---
times = data_lines[0]
estimated_median = data_lines[1]
data = data_lines[2:]  # itt minden elem egy sor = lista

# --- 3: transzponálás ---
transposed_data = list(zip(*data))  # tuple-öket ad vissza

# --- 4: kimeneti sorok előállítása ---
output_rows = []
for i, row in enumerate(transposed_data):
    row = [float(x) for x in row]  # string -> float
    computed_median = stats.median(row)
    # sor: időpont, becsült medián, számolt medián, adatok
    new_row = [times[i], estimated_median[i], f"{computed_median:.6f}"] + [str(x) for x in row]
    output_rows.append("\t".join(new_row))

output_file = filename+".median"
print(output_file)
# --- 5: fájl írása ---
with open(output_file, "w") as f:
    for c in comments:
        f.write(c + "\n")
    for row in output_rows:
        f.write(row + "\n")