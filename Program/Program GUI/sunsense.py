import tkinter as tk
from tkinter import ttk, messagebox
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import serial
import serial.tools.list_ports
import threading
import time
from collections import deque
from ttkthemes import ThemedStyle

# Buffer data
MAX_POINTS = 50
time_data = list(range(MAX_POINTS))
temp_data = deque([0]*MAX_POINTS, maxlen=MAX_POINTS)
humi_data = deque([0]*MAX_POINTS, maxlen=MAX_POINTS)
light_data = deque([0]*MAX_POINTS, maxlen=MAX_POINTS)

class SmartDryerGUI:
    def __init__(self, root, serial_port):
        self.root = root
        self.root.title("SUNSENSE: Smart Drying Rack")
        self.root.geometry("1100x750")
        self.root.configure(bg="#1f1f1f")

        self.status_var = tk.StringVar(value="Status: Menunggu data...")

        self.serial_port = serial_port
        self.serial_conn = None
        self.connect_serial()

        self.create_widgets()
        self.update_gui()
        threading.Thread(target=self.read_serial, daemon=True).start()

    def connect_serial(self):
        try:
            self.serial_conn = serial.Serial(self.serial_port, 9600, timeout=1)
            self.status_var.set("✅ Terhubung ke Arduino")
        except Exception as e:
            self.status_var.set(f"❌ Gagal terhubung: {str(e)}")
            self.show_error(f"Tidak dapat terhubung ke Arduino:\n{str(e)}")

    def create_widgets(self):
        style = ThemedStyle(self.root)
        style.set_theme("equilux")
        style.configure('Card.TFrame', background='#2a2a2a')
        style.configure('TLabel', background='#2a2a2a', foreground='white', font=('Segoe UI', 11))
        style.configure('TButton', font=('Segoe UI', 11), background='#333', foreground='white')

        header = ttk.Label(self.root, text="SUNSENSE: Smart Drying Rack", font=('Segoe UI', 22, 'bold'), foreground="#FFD700")
        header.pack(pady=10)

        main_frame = ttk.Frame(self.root)
        main_frame.pack(fill='both', expand=True, padx=10, pady=10)

        left_frame = ttk.Frame(main_frame)
        left_frame.pack(side='left', fill='y', padx=10)

        self.create_sensor_card(left_frame, "MODE", "mode_val")
        self.create_sensor_card(left_frame, "SUHU", "temp_val")
        self.create_sensor_card(left_frame, "KELEMBAPAN", "humi_val")
        self.create_sensor_card(left_frame, "CAHAYA", "light_val")
        self.create_sensor_card(left_frame, "RAIN", "rain_val")
        self.create_sensor_card(left_frame, "POSISI", "pos_val")

        control_frame = ttk.Frame(left_frame)
        control_frame.pack(fill='x', pady=20)

        self.mode_btn = self.create_button(control_frame, "SWITCH MODE", self.toggle_mode)
        self.pos_btn = self.create_button(control_frame, "GERAKKAN JEMURAN", self.toggle_position, state='disabled')

        right_frame = ttk.Frame(main_frame)
        right_frame.pack(side='right', fill='both', expand=True)

        self.fig = Figure(figsize=(6.5, 3.8), dpi=100)
        self.fig.patch.set_facecolor('#1f1f1f')
        self.ax1 = self.fig.add_subplot(311)
        self.ax2 = self.fig.add_subplot(312)
        self.ax3 = self.fig.add_subplot(313)

        for ax, label in zip([self.ax1, self.ax2, self.ax3], ['Suhu (°C)', 'Kelembapan (%)', 'Cahaya']):
            ax.set_facecolor('#2a2a2a')
            ax.grid(True, linestyle='--', alpha=0.4, color='gray')
            ax.tick_params(colors='white')
            ax.set_ylabel(label, color='white')
            ax.set_xlabel('Waktu', color='white')

        self.line1, = self.ax1.plot(time_data, temp_data, '#FF6B6B')
        self.line2, = self.ax2.plot(time_data, humi_data, '#4ECDC4')
        self.line3, = self.ax3.plot(time_data, light_data, '#FFD700')

        self.canvas = FigureCanvasTkAgg(self.fig, master=right_frame)
        self.canvas.get_tk_widget().pack(fill='both', expand=True)
        self.fig.tight_layout(pad=2)

        status = ttk.Label(self.root, textvariable=self.status_var, font=('Segoe UI', 10), foreground='white')
        status.pack(fill='x', padx=10, pady=5)

    def create_sensor_card(self, parent, title, var_name):
        frame = ttk.Frame(parent, style='Card.TFrame', padding=8)
        frame.pack(fill='x', pady=4)

        label_title = ttk.Label(frame, text=title, font=('Segoe UI', 13, 'bold'))
        label_title.pack(anchor='w')

        label_value = ttk.Label(frame, text="N/A", font=('Segoe UI', 16, 'bold'), foreground='#FFD700')
        label_value.pack(anchor='w')

        setattr(self, var_name, label_value)

    def create_button(self, parent, text, command, state='normal'):
        btn = tk.Button(parent, text=text, command=command, bg="#444", fg="white",
                        font=("Segoe UI", 11, "bold"), relief="flat", activebackground="#666",
                        activeforeground="white")
        btn.pack(fill='x', pady=5)
        btn.config(state=state)
        return btn

    def parse_data(self, line):
        try:
            data = dict(part.split(':') for part in line.strip().split('|') if ':' in part)

            if all(key in data for key in ['MODE', 'T', 'H', 'L', 'D', 'R']):
                self.mode_val.config(text=data['MODE'])
                self.temp_val.config(text=f"{data['T']} °C")
                self.humi_val.config(text=f"{data['H']} %")
                self.light_val.config(text=data['L'])
                self.pos_val.config(text=data['D'])

                rain_status = data['R'].upper()
                self.rain_val.config(text="Rain" if rain_status == "RAINING" else "No Rain")

                temp_data.append(float(data['T']))
                humi_data.append(float(data['H']))
                light_data.append(int(data['L']))

                self.mode_btn.config(text="SWITCH KE OTOMATIS" if data['MODE'] == 'MANUAL' else "SWITCH KE MANUAL")
                self.pos_btn.config(state='normal' if data['MODE'] == 'MANUAL' else 'disabled')

                self.status_var.set(f"🔄 Data diperbarui: {time.strftime('%H:%M:%S')}")
        except Exception as e:
            print(f"Error parsing data: {e}")

    def toggle_mode(self):
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.write(b'm')
            self.status_var.set("🔄 Mode diubah")

    def toggle_position(self):
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.write(b'c')
            self.status_var.set("🔄 Posisi jemuran diubah")

    def read_serial(self):
        while True:
            if self.serial_conn and self.serial_conn.is_open and self.serial_conn.in_waiting:
                try:
                    line = self.serial_conn.readline().decode('utf-8').strip()
                    if line.startswith('MODE:'):
                        self.root.after(0, self.parse_data, line)
                except:
                    pass
            time.sleep(0.1)

    def update_gui(self):
        self.update_plots()
        self.root.after(1000, self.update_gui)

    def update_plots(self):
        self.line1.set_ydata(temp_data)
        self.line2.set_ydata(humi_data)
        self.line3.set_ydata(light_data)

        for ax in [self.ax1, self.ax2, self.ax3]:
            ax.relim()
            ax.autoscale_view()

        self.canvas.draw_idle()

    def show_error(self, message):
        messagebox.showerror("Error", message)

def select_port_window():
    port_window = tk.Tk()
    port_window.title("Pilih Port Serial")
    port_window.geometry("350x150")
    port_window.configure(bg="#2a2a2a")

    ttk.Label(port_window, text="Pilih Port Serial:", font=('Segoe UI', 12)).pack(pady=10)

    port_combo = ttk.Combobox(port_window, state="readonly", width=30)
    port_combo.pack()

    ports = list(serial.tools.list_ports.comports())
    port_combo['values'] = [port.device for port in ports]

    if ports:
        port_combo.current(0)

    def connect():
        selected_port = port_combo.get()
        if not selected_port:
            messagebox.showwarning("Peringatan", "Pilih port terlebih dahulu!")
            return
        port_window.destroy()
        main_window = tk.Tk()
        app = SmartDryerGUI(main_window, selected_port)
        main_window.mainloop()

    ttk.Button(port_window, text="Connect", command=connect).pack(pady=15)
    port_window.mainloop()

if __name__ == "__main__":
    select_port_window()
