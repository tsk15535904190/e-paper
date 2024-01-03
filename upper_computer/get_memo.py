import configparser

def get_memo_info():
    config = configparser.ConfigParser()
    config.read('./config.ini',encoding='utf-8')
    path = config.get('memorandum', 'path')
    try:
        with open(path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
            line_num = len(lines)
            for line_number, line_content in enumerate(lines, start=1):
                print(f'Line {line_number}: {line_content.strip()}')
    except FileNotFoundError:
        print(f"File '{path}' not found.")
        return 0 
    except Exception as e:
        print(f"An error occurred: {e}")
        return 0 
