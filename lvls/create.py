import os
import csv

LVLDIR = "."

def main():
    print(f"Creating a new level!")
    os.makedirs(LVLDIR, exist_ok=True)
    existing_files = [f for f in os.listdir(LVLDIR) if f.endswith(".csv")]
    index = len(existing_files)
    name = input("- Name: ").strip().replace(" ", "_")
    width = int(input("- Width: "))
    height = int(input("- Height: "))
    filename = f"{index}-{name}-{width}x{height}.csv"
    filepath = os.path.join(LVLDIR, filename)
    with open(filepath, "w", newline="") as f:
        writer = csv.writer(f)
        for _ in range(height):
            writer.writerow([0] * width)
    print(f"Succesfully created a new level: {filepath}")

if __name__ == "__main__":
    main()
