import os
import pandas as pd

# Get the directory where the script is located
script_dir = os.path.dirname(os.path.abspath(__file__))

# Paths for input and output files
input_folder = os.path.join(script_dir, '..', 'Game_data')
castle_output_file = os.path.join(script_dir, '..', 'Game_Analysis', 'Castle', 'castle_data.xlsx')
enemies_output_folder = os.path.join(script_dir, '..', 'Game_Analysis', 'Enemies')

def clean_sheet(df, subset_columns):
    """
    Clean a single DataFrame by removing duplicates and handling missing values.
    """
    # Drop rows with any missing values
    df = df.dropna()

    # Remove duplicate rows based on the specified subset of columns
    df = df.drop_duplicates(subset=subset_columns)

    # Convert data types if necessary (Example: converting 'date' and 'amount')
    if 'date' in df.columns:
        df['date'] = pd.to_datetime(df['date'], errors='coerce')

    if 'amount' in df.columns:
        df['amount'] = pd.to_numeric(df['amount'], errors='coerce')

    # Fill missing values with a specified value
    df = df.fillna(0)

    return df

def clean_excel_data(input_file, output_file, subset_columns, multiple_sheets=False):
    try:
        if multiple_sheets:
            # Load the Excel file with multiple sheets
            xl = pd.ExcelFile(input_file, engine='openpyxl')

            # Dictionary to store cleaned data for each sheet
            cleaned_sheets = {}

            # Iterate through each sheet
            for sheet_name in xl.sheet_names:
                print(f"Cleaning sheet: {sheet_name}")

                # Read the sheet into a DataFrame
                df = xl.parse(sheet_name)

                # Clean the DataFrame
                cleaned_df = clean_sheet(df, subset_columns)

                # Store the cleaned DataFrame in the dictionary
                cleaned_sheets[sheet_name] = cleaned_df

            # Save all cleaned sheets to a new Excel file
            with pd.ExcelWriter(output_file, engine='openpyxl') as writer:
                for sheet_name, cleaned_df in cleaned_sheets.items():
                    cleaned_df.to_excel(writer, sheet_name=sheet_name, index=False)

            print(f"Cleaned data has been saved to {output_file}")

        else:
            # Load the Excel file with a single sheet
            df = pd.read_excel(input_file, engine='openpyxl')

            # Clean the DataFrame
            cleaned_df = clean_sheet(df, subset_columns)

            # Save the cleaned data back to an Excel file
            cleaned_df.to_excel(output_file, index=False, engine='openpyxl')
            print(f"Cleaned data has been saved to {output_file}")

    except Exception as e:
        print(f"An error occurred: {e}")

# Specify the input and output file paths and other parameters
input_file_multiple_sheets = os.path.join(enemies_output_folder, 'all_enemies_combined.xlsx')
output_file_multiple_sheets = os.path.join(enemies_output_folder, 'cleaned_all_enemies_combined.xlsx')
subset_columns_multiple_sheets = ['Distance', 'Current Health', 'Melting Rate', 'Ice Accumulated', 'Status']

input_file_single_sheet = castle_output_file
output_file_single_sheet = os.path.join(script_dir, '..', 'Game_Analysis', 'Castle', 'cleaned_castle_data.xlsx')
subset_columns_single_sheet = ['Health', 'Status', 'Ice Accumulated']

# Call the function for multiple sheets
clean_excel_data(input_file_multiple_sheets, output_file_multiple_sheets, subset_columns_multiple_sheets, multiple_sheets=True)

# Call the function for a single sheet
clean_excel_data(input_file_single_sheet, output_file_single_sheet, subset_columns_single_sheet, multiple_sheets=False)
