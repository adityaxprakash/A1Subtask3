# Quantitative Trading Simulator and Analyzer
In this project, we aim to implement pre-existing strategies for quantitative trading and learn how various indicators and algorithms are used in the finance world.

## Authors
* Ansh Agrawal (2022CS51135)
* Aditya Prakash (2022CS11595)

## Installation
You should have the jugaad-data library installed. You can do so by running the following on any terminal.
```bash
    pip install jugaad-data
```
## Strategies Implemented
We have implemented several well known strategies like:
* **Momentum Based** Strategies like
    * Basic Strategy involving montonic price increase/decrease over a fixed period of days.
    * Moving Averages(DMA) with and without maximum hold period
    * Indicators like Moving Average Convergence/Divergence(MACD), Relative Strength Index(RSI) and Average Directional Index(ADX).
* **Linear Regression** based strategy using past trends
* **Mean-Reverting Pairs** Trading 

## Usage
It is recommended to use this on a UNIX based system. Clone the repository into your local system and run any of the following commands on the terminal:
```bash
    make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"
    make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
    make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
    make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
    make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
    make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
    make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
    make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"
```
To clean existing built and data files, run:
```bash
    make clean
```
## Design Choices and Optimizations Made
We have created a modular codebase for this project, including but not limited to different directories for build and source files, abstraction between different components of the code.

We have a virtual base _strategy_ class which encapsulates the common attributes in every strategy. Each particular strategy is derived from this and has several auxiliary attributes particular to it. The historical data is extracted using a python script depending on the strategy to be implemented. 

Other optimizations include:
* We have used prefix sums to calculate rolling means and standard deviation instead of iterating through n days thus reducing the time complexity order by n. 
* For maintaining open positions, we have employed the use of deques to make all operations as _O(1)_. 
* We have used Gauss-Jordan elimination to compute inverse of a matrix instead of the traditional adjoint method. This leads to better numerical stability and better time complexity (polynomial instead of factorial)
* We have employed the use of OpenMP in C++ to _parallelize_ the running the best of all strategy.

## Insights
We played around with the values of the parameters to get a feel of how different strategies behave. We plotted all such data as follows:
<img src="# Quantitative Trading Simulator and Analyzer
In this project, we aim to implement pre-existing strategies for quantitative trading and learn how various indicators and algorithms are used in the finance world.

## Authors
* Ansh Agrawal (2022CS51135)
* Aditya Prakash (2022CS11595)

## Installation
You should have the jugaad-data library installed. You can do so by running the following on any terminal.
```bash
    pip install jugaad-data
```
## Strategies Implemented
We have implemented several well known strategies like:
* **Momentum Based** Strategies like
    * Basic Strategy involving montonic price increase/decrease over a fixed period of days.
    * Moving Averages(DMA) with and without maximum hold period
    * Indicators like Moving Average Convergence/Divergence(MACD), Relative Strength Index(RSI) and Average Directional Index(ADX).
* **Linear Regression** based strategy using past trends
* **Mean-Reverting Pairs** Trading 

## Usage
It is recommended to use this on a UNIX based system. Clone the repository into your local system and run any of the following commands on the terminal:
```bash
    make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"
    make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
    make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
    make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
    make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
    make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
    make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
    make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
    make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"
```
To clean existing built and data files, run:
```bash
    make clean
```
## Design Choices and Optimizations Made
We have created a modular codebase for this project, including but not limited to different directories for build and source files, abstraction between different components of the code.

We have a virtual base _strategy_ class which encapsulates the common attributes in every strategy. Each particular strategy is derived from this and has several auxiliary attributes particular to it. The historical data is extracted using a python script depending on the strategy to be implemented. 

Other optimizations include:
* We have used prefix sums to calculate rolling means and standard deviation instead of iterating through n days thus reducing the time complexity order by n. 
* For maintaining open positions, we have employed the use of deques to make all operations as _O(1)_. 
* We have used Gauss-Jordan elimination to compute inverse of a matrix instead of the traditional adjoint method. This leads to better numerical stability and better time complexity (polynomial instead of factorial)
* We have employed the use of OpenMP in C++ to _parallelize_ the running the best of all strategy.

## Insights
We played around with the values of the parameters to get a feel of how different strategies behave. We plotted all such data as follows:
<img src="[https://github.com/AnshAgrawal04/Images/issues/1#issue-2132432879](https://raw.githubusercontent.com/adityaxprakash/Benchmark-code-for-A1/main/RSI.jpeg)">
*Jugaad-data does not ewrwefewf

## Mean Reverting Pairs Strategy using Stop-Loss
We incorporated stop-loss into our mean reverting pairs to prevent losses if the stock behaves unexpectedly. When the stock crosses the positive stop-threshold, buying it back reduces our possibility of loss. Meanwhile, the stock crossing the negative stop-threshold also indicates a closing signal. But this strategy is on top of our normal strategy. In case of conflicts, i.e. BUY Sell


*Jugaad-data does not ewrwefewf

## Mean Reverting Pairs Strategy using Stop-Loss
We incorporated stop-loss into our mean reverting pairs to prevent losses if the stock behaves unexpectedly. When the stock crosses the positive stop-threshold, buying it back reduces our possibility of loss. Meanwhile, the stock crossing the negative stop-threshold also indicates a closing signal. But this strategy is on top of our normal strategy. In case of conflicts, i.e. BUY Sell

