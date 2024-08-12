import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Get the directory where the script is located
script_dir = os.path.dirname(os.path.abspath(__file__))

# Directories
castle_output_dir = os.path.join(script_dir, '..', 'Game_Analysis', 'Castle', 'Castle_Visuals')
enemies_output_dir = os.path.join(script_dir, '..', 'Game_Analysis', 'Enemies', 'Enemies_Visuals')
os.makedirs(castle_output_dir, exist_ok=True)
os.makedirs(enemies_output_dir, exist_ok=True)

# Load the castle data
castle_data = pd.read_excel(os.path.join(script_dir, '..', 'Game_Analysis', 'Castle', 'cleaned_castle_data.xlsx'), engine='openpyxl')

# Map 'Status' values to numeric values for castle data
status_mapping = {'Active': 1, 'Frosted': -1, 'Killed': 0}
castle_data['Status Numeric'] = castle_data['Status'].map(status_mapping)

# Castle Visualizations
# Line Chart: Health Over Time
plt.figure(figsize=(10, 6))
plt.plot(castle_data['Current Time Step'], castle_data['Health'], marker='o')
plt.xlabel('Current Time Step')
plt.ylabel('Health')
plt.title('Health Over Time')
plt.grid(True)
plt.savefig(os.path.join(castle_output_dir, 'health_over_time.png'))
plt.close()

# Line Chart: Ice Accumulated Over Time
plt.figure(figsize=(10, 6))
plt.scatter(castle_data['Current Time Step'], castle_data['Ice Accumulated'], marker='o', color='orange')
plt.xlabel('Current Time Step')
plt.ylabel('Ice Accumulated')
plt.title('Ice Accumulated Over Time')
plt.grid(True)
plt.savefig(os.path.join(castle_output_dir, 'ice_accumulated_over_time.png'))
plt.close()

# Line Chart: Status Over Time
plt.figure(figsize=(10, 6))
plt.scatter(castle_data['Current Time Step'], castle_data['Status Numeric'], marker='o', color='green')
plt.xlabel('Current Time Step')
plt.ylabel('Status')
plt.title('Status Over Time')
plt.grid(True)
plt.savefig(os.path.join(castle_output_dir, 'status_over_time.png'))
plt.close()

# Scatter Plot: Health vs. Ice Accumulated
plt.figure(figsize=(10, 6))
plt.scatter(castle_data['Health'], castle_data['Ice Accumulated'], alpha=0.7)
plt.xlabel('Health')
plt.ylabel('Ice Accumulated')
plt.title('Health vs. Ice Accumulated')
plt.grid(True)
plt.savefig(os.path.join(castle_output_dir, 'health_vs_ice_accumulated.png'))
plt.close()

# Heatmap: Correlation Matrix
plt.figure(figsize=(12, 8))
numeric_castle_data = castle_data.select_dtypes(include=[float, int])
corr = numeric_castle_data.corr()
sns.heatmap(corr, annot=True, cmap='coolwarm')
plt.title('Correlation Matrix')
plt.savefig(os.path.join(castle_output_dir, 'correlation_matrix.png'))
plt.close()

# Load the enemies data
excel_file = os.path.join(script_dir, '..', 'Game_Analysis', 'Enemies', 'cleaned_all_enemies_combined.xlsx')
xl = pd.ExcelFile(excel_file, engine='openpyxl')

# Function to visualize data from a specific sheet and save the plots
def visualize_and_save_sheet(sheet_name):
    data = xl.parse(sheet_name)

    # Map 'Status' values to numeric values for enemies data
    status_mapping = {'Active': 1, 'Frosted': -1, 'Killed': 0}
    data['Status Numeric'] = data['Status'].map(status_mapping)

    # Line Chart: Current Health Over Time
    plt.figure(figsize=(10, 6))
    plt.plot(data['Time Step'], data['Current Health'], marker='o')
    plt.xlabel('Time Step')
    plt.ylabel('Current Health')
    plt.title(f'Current Health Over Time ({sheet_name})')
    plt.grid(True)
    plt.savefig(os.path.join(enemies_output_dir, f'{sheet_name}_current_health_over_time.png'))
    plt.close()

    # Scatter Plot: Health vs. Ice Accumulated
    plt.figure(figsize=(10, 6))
    plt.scatter(data['Current Health'], data['Ice Accumulated'], alpha=0.7)
    plt.xlabel('Current Health')
    plt.ylabel('Ice Accumulated')
    plt.title(f'Current Health vs. Ice Accumulated ({sheet_name})')
    plt.grid(True)
    plt.savefig(os.path.join(enemies_output_dir, f'{sheet_name}_health_vs_ice_accumulated.png'))
    plt.close()

    # Line Chart: Status Over Time
    plt.figure(figsize=(10, 6))
    plt.plot(data['Time Step'], data['Status Numeric'], marker='o', color='green')
    plt.xlabel('Time Step')
    plt.ylabel('Status')
    plt.title(f'Status Over Time ({sheet_name})')
    plt.grid(True)
    plt.savefig(os.path.join(enemies_output_dir, f'{sheet_name}_status_over_time.png'))
    plt.close()

    # Heatmap: Correlation Matrix
    plt.figure(figsize=(12, 8))
    numeric_data = data.select_dtypes(include=[float, int])
    corr = numeric_data.corr()
    sns.heatmap(corr, annot=True, cmap='coolwarm')
    plt.title(f'Correlation Matrix ({sheet_name})')
    plt.savefig(os.path.join(enemies_output_dir, f'{sheet_name}_correlation_matrix.png'))
    plt.close()

# Iterate through each sheet in the enemies file and visualize the data
for sheet in xl.sheet_names:
    visualize_and_save_sheet(sheet)
