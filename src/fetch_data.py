from jugaad_data.nse import stock_df
from dateutil.relativedelta import relativedelta
import pandas as pd
from datetime import datetime
import sys

def write_stock_data(symbol_name, start_date, end_date, n, output_file):
    start_date = datetime.strptime(start_date, "%d/%m/%Y")
    end_date = datetime.strptime(end_date, "%d/%m/%Y")
    columns=['DATE','CLOSE','OPEN','HIGH','LOW','NO OF TRADES','PREV. CLOSE','VWAP']
    df = get_stock_data(symbol_name, start_date, end_date, n,columns)
    df=df.drop_duplicates(subset=['DATE'],inplace=False)
    df.to_csv(output_file,index=False)

def get_stock_data(symbol_name, start_date, end_date,n,columns):
    pre_start_date = start_date - relativedelta(days=2*n+10)
    pre_end_date = start_date - relativedelta(days=1)
    try:
        df = stock_df(symbol=symbol_name, from_date=start_date, to_date=end_date, series="EQ").iloc[::-1]
    except:
        df=pd.DataFrame(columns=columns)
    try:
        df_old=stock_df(symbol=symbol_name, from_date=pre_start_date, to_date=pre_end_date, series="EQ").iloc[::-1].tail(n)
    except:
        df_old=pd.DataFrame(columns=columns)
    df = pd.concat([df_old,df],axis=0)
    return df[columns]

def main():
    if len(sys.argv) == 6:
        symbol_name=sys.argv[1]
        start_date=sys.argv[2]
        end_date=sys.argv[3]
        n=int(sys.argv[4])
        file_name = sys.argv[5]
        write_stock_data(symbol_name, start_date, end_date, n, file_name)
    
    else:
        print("Incorrect command line arguments provided.")

if __name__ == "__main__":
    main()
