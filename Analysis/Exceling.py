import pandas as pd
import re
import os

# Function to extract castle data
def extract_castle_data(data):
    castle_data = {}
    castle_data['Current Time Step'] = int(re.search(r'Current Time Step: (\d+)', data).group(1))
    castle_data['Health'] = float(re.search(r'Health: ([\d\.]+)', data).group(1))
    castle_data['Max Health'] = float(re.search(r'Max Health: ([\d\.]+)', data).group(1))
    castle_data['Health Threshold'] = float(re.search(r'Health Threshold: ([\d\.]+)', data).group(1))
    castle_data['Status'] = re.search(r'Status: (\w+)', data).group(1)
    castle_data['Freezing Threshold'] = float(re.search(r'Freezing Threshold: ([\d\.]+)', data).group(1))
    castle_data['Ice Accumulated'] = float(re.search(r'Ice Accumulated: ([\d\.]+)', data).group(1))
    castle_data['Power'] = re.search(r'Power: (\d+)', data).group(1)
    castle_data['N'] = re.search(r'N: (\d+)', data).group(1)
    castle_data['Number of Super Soldiers'] = re.search(r'Number of Super Soldiers: (\d+)', data).group(1)
    return castle_data

# Function to extract enemies data
def extract_enemies_data(data):
    enemies_data = []
    current_time_step = int(re.search(r'Current Time Step: (\d+)', data).group(1))
    lines = data.split('\n')
    enemy_data = None

    for line in lines:
        if line.startswith('ID:'):
            if enemy_data:
                enemies_data.append(enemy_data)
            enemy_data = {'Time Step': current_time_step}
            enemy_data['ID'] = re.search(r'ID: (\d+)', line).group(1)
        elif enemy_data is not None:
            if 'Arrival Time:' in line:
                enemy_data['Arrival Time'] = re.search(r'Arrival Time: (\d+)', line).group(1)
            elif 'Distance:' in line:
                enemy_data['Distance'] = float(re.search(r'Distance: ([\d\.]+)', line).group(1))
            elif 'Max Health:' in line:
                enemy_data['Max Health'] = float(re.search(r'Max Health: ([\d\.]+)', line).group(1))
            elif 'Current Health:' in line:
                enemy_data['Current Health'] = float(re.search(r'Current Health: ([\d\.]+)', line).group(1))
            elif 'Speed:' in line:
                enemy_data['Speed'] = float(re.search(r'Speed: ([\d\.]+)', line).group(1))
            elif 'Power:' in line:
                enemy_data['Power'] = re.search(r'Power: (\d+)', line).group(1)
            elif 'Reload Period:' in line:
                enemy_data['Reload Period'] = re.search(r'Reload Period: (\d+)', line).group(1)
            elif 'Reload Counter:' in line:
                enemy_data['Reload Counter'] = re.search(r'Reload Counter: (\d+)', line).group(1)
            elif 'Ice Accumulated:' in line:
                enemy_data['Ice Accumulated'] = float(re.search(r'Ice Accumulated: ([\d\.]+)', line).group(1))
            elif 'Freezing Threshold:' in line:
                enemy_data['Freezing Threshold'] = float(re.search(r'Freezing Threshold: ([\d\.]+)', line).group(1))
            elif 'Melting Rate:' in line:
                enemy_data['Melting Rate'] = float(re.search(r'Melting Rate: ([\d\.]+)', line).group(1))
            elif 'Status:' in line:
                enemy_data['Status'] = re.search(r'Status: (\w+)', line).group(1)
            elif 'Type:' in line:
                enemy_data['Type'] = re.search(r'Type: (\w+)', line).group(1)
            elif 'First Shot time:' in line:
                enemy_data['First Shot time'] = re.search(r'First Shot time: (\d+)', line).group(1)
            elif 'Killed time:' in line:
                enemy_data['Killed time'] = re.search(r'Killed time: (\d+)', line).group(1)

    if enemy_data:
        enemies_data.append(enemy_data)

    return enemies_data

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Folder containing text files
input_folder = os.path.join(script_dir, '..', 'Game_data')

# Paths for output files
castle_output_file = os.path.join(script_dir, '..', 'Game_Analysis', 'Castle', 'castle_data.xlsx')
#super_soldier_output_file = os.path.join(script_dir, '..', 'Game_Analysis', 'super_soldiers_data.xlsx')
enemies_output_folder = os.path.join(script_dir, '..', 'Game_Analysis', 'Enemies')

# Initialize lists to store data
castle_data_list = []
super_soldier_data_list = []
enemies_data_list = []

# Process each text file in the folder
for filename in os.listdir(input_folder):
    if filename.endswith('.txt'):
        file_path = os.path.join(input_folder, filename)

        try:
            with open(file_path, 'r') as file:
                data = file.read()

            # Extract data
            castle_data = extract_castle_data(data)
            #super_soldier_data = extract_super_soldier_data(data)
            enemies_data = extract_enemies_data(data)

            # Append data to lists
            castle_data_list.append(castle_data)
            #super_soldier_data_list.extend(super_soldier_data)
            enemies_data_list.extend(enemies_data)
        except Exception as e:
            print(f"Error processing file {filename}: {e}")

# Create DataFrames
castle_df = pd.DataFrame(castle_data_list)
#super_soldier_df = pd.DataFrame(super_soldier_data_list)
enemies_df = pd.DataFrame(enemies_data_list)

# Ensure 'Time Step' columns are integers and other relevant columns are floats
if not enemies_df.empty:
    enemies_df['Time Step'] = enemies_df['Time Step'].astype(int)
    enemies_df['Distance'] = enemies_df['Distance'].astype(float)
    enemies_df['Max Health'] = enemies_df['Max Health'].astype(float)
    enemies_df['Current Health'] = enemies_df['Current Health'].astype(float)
    enemies_df['Speed'] = enemies_df['Speed'].astype(float)
    enemies_df['Ice Accumulated'] = enemies_df['Ice Accumulated'].astype(float)
    enemies_df['Freezing Threshold'] = enemies_df['Freezing Threshold'].astype(float)
    enemies_df['Melting Rate'] = enemies_df['Melting Rate'].astype(float)
    enemies_df.sort_values(by='Time Step', ascending=True, inplace=True)

# Sort DataFrames by 'Time Step'
castle_df.sort_values(by='Current Time Step', ascending=True, inplace=True)

# Save castle and super soldier data to separate Excel files
castle_df.to_excel(castle_output_file, index=False)
#super_soldier_df.to_excel(super_soldier_output_file, index=False)

# Create the output folder for enemies if it does not exist
if not os.path.exists(enemies_output_folder):
    os.makedirs(enemies_output_folder)

# Initialize an ExcelWriter object for the combined file
combined_output_file = os.path.join(enemies_output_folder, 'all_enemies_combined.xlsx')
combined_writer = pd.ExcelWriter(combined_output_file, engine='xlsxwriter')

# Sort the groups by the combined key of (enemy_type, enemy_id)
sorted_groups = sorted(enemies_df.groupby(['Type', 'ID']), key=lambda x: int(x[0][1]))

# Save enemies data to separate Excel files by 'ID' and 'Type' and also to the combined file
for (enemy_type, enemy_id), group_df in sorted_groups:
    sheet_name = f'{enemy_id}_{enemy_type}'
    output_file = os.path.join(enemies_output_folder, f'{sheet_name}.xlsx')

    # Save to individual Excel files
    group_df.to_excel(output_file, index=False, sheet_name=sheet_name)

    # Save to the combined Excel file
    group_df.to_excel(combined_writer, index=False, sheet_name=sheet_name)

# Save the combined file
combined_writer._save()

print("Data has been successfully saved to Excel files, including the combined file.")
