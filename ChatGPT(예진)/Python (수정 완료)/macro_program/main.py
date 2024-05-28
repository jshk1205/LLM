import threading
import time
from flask import Flask, request, render_template
import pyautogui

app = Flask(__name__)
macro_thread = None


def send_keys(text, interval):
    print(f"Start the keystroke macro. Repeat {text} every {interval} seconds.")
    try:
        while True:
            pyautogui.write(text)
            time.sleep(interval)
    except KeyboardInterrupt:
        print("\nExit the program.")


@app.route('/', methods=['GET', 'POST'])
def index():
    global macro_thread

    if request.method == 'POST':
        text = request.form['text']
        interval = float(request.form['interval'])

        if macro_thread and macro_thread.is_alive():
            return "The macro is already running."

        macro_thread = threading.Thread(target=send_keys, args=(text, interval))
        macro_thread.start()

        return "The macro has started. Shut down the program to stop the macro."

    return render_template('index.html')


@app.route('/stop', methods=['GET'])
def stop_macro():
    global macro_thread

    if macro_thread and macro_thread.is_alive():
        macro_thread.join()
        return "Macro stopped."
    else:
        return "macro is not running."


if __name__ == "__main__":
    app.run(debug=True)
