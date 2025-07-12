import os
import shutil
import tkinter as tk
from tkinter import filedialog, messagebox
import ctypes
import winreg
import sys

if sys.platform != "win32":
    raise SystemExit("This installer only works on Windows.")

INSTALL_DIR = "C:\\NuCMDs"

def add_to_path():
    with winreg.OpenKey(winreg.HKEY_CURRENT_USER, "Environment", 0, winreg.KEY_ALL_ACCESS) as key:
        try:
            current_path, _ = winreg.QueryValueEx(key, "Path")
        except FileNotFoundError:
            current_path = ""
        paths = current_path.split(";")
        if INSTALL_DIR.lower() not in [p.lower() for p in paths]:
            paths.append(INSTALL_DIR)
            winreg.SetValueEx(key, "Path", 0, winreg.REG_EXPAND_SZ, ";".join(paths))

def remove_from_path():
    with winreg.OpenKey(winreg.HKEY_CURRENT_USER, "Environment", 0, winreg.KEY_ALL_ACCESS) as key:
        try:
            current_path, _ = winreg.QueryValueEx(key, "Path")
        except FileNotFoundError:
            return
        paths = [p for p in current_path.split(";") if p.lower() != INSTALL_DIR.lower()]
        winreg.SetValueEx(key, "Path", 0, winreg.REG_EXPAND_SZ, ";".join(paths))

def browse_folder():
    folder = filedialog.askdirectory()
    if folder:
        folder_var.set(folder)

def install():
    src = folder_var.get()
    if not os.path.isdir(src):
        status_label.config(text="❌ Invalid folder", fg="red")
        return
    try:
        if os.path.exists(INSTALL_DIR):
            shutil.rmtree(INSTALL_DIR)
        shutil.copytree(src, INSTALL_DIR)
        add_to_path()
        status_label.config(text="✅ Installed. Restart terminal to use NuCMDs!", fg="lightgreen")
    except Exception as e:
        status_label.config(text=f"❌ Failed: {e}", fg="red")

def uninstall():
    try:
        if os.path.exists(INSTALL_DIR):
            shutil.rmtree(INSTALL_DIR)
        remove_from_path()
        status_label.config(text="✅ Uninstalled. Restart terminal to finish.", fg="lightgreen")
    except Exception as e:
        status_label.config(text=f"❌ Failed: {e}", fg="red")

def toggle_theme():
    global dark_mode
    dark_mode = not dark_mode
    apply_theme()

root = tk.Tk()
root.geometry("460x320")
root.overrideredirect(True)  
root.eval('tk::PlaceWindow . center')
ctypes.windll.user32.SetProcessDPIAware()
root.wm_attributes("-alpha", 0.95)

def start_move(event):
    root.x = event.x
    root.y = event.y

def stop_move(event):
    root.x = None
    root.y = None

def do_move(event):
    deltax = event.x - root.x
    deltay = event.y - root.y
    x = root.winfo_x() + deltax
    y = root.winfo_y() + deltay
    root.geometry(f"+{x}+{y}")

dark_mode = True
titlebar = tk.Frame(root, bg="#2D89EF", height=30)
titlebar.pack(fill="x")
titlebar.bind("<ButtonPress-1>", start_move)
titlebar.bind("<ButtonRelease-1>", stop_move)
titlebar.bind("<B1-Motion>", do_move)

title_label = tk.Label(titlebar, text=" NuCMDs Installer", font=("Segoe UI", 10, "bold"),
                       bg="#2D89EF", fg="white")
title_label.pack(side="left", padx=10)

close_btn = tk.Button(titlebar, text="✕", command=root.destroy, relief="flat",
                      bg="#2D89EF", fg="white", activebackground="red",
                      font=("Segoe UI", 10, "bold"))
close_btn.pack(side="right", padx=8)
frame = tk.Frame(root)
frame.pack(pady=18, expand=True)

tk.Label(frame, text="Select Folder With Your NuCMDs .exe Commands",
         font=("Segoe UI", 10, "bold")).pack(pady=(0, 6))
folder_var = tk.StringVar()
folder_entry = tk.Entry(frame, textvariable=folder_var, width=48, font=("Consolas", 10), relief="flat")
folder_entry.pack(ipady=5)
browse_btn = tk.Button(frame, text="Browse Folder", command=browse_folder, font=("Segoe UI", 9), relief="flat")
browse_btn.pack(pady=6)
install_btn = tk.Button(frame, text="Install NuCMDs", command=install, font=("Segoe UI", 10, "bold"), relief="flat", width=20)
install_btn.pack(pady=4)
uninstall_btn = tk.Button(frame, text="Uninstall NuCMDs", command=uninstall, font=("Segoe UI", 10, "bold"), relief="flat", width=20)
uninstall_btn.pack(pady=4)
theme_toggle = tk.Button(frame, text="Toggle Theme", command=toggle_theme, font=("Segoe UI", 9), relief="flat")
theme_toggle.pack(pady=8)
status_label = tk.Label(root, text="", font=("Consolas", 9))
status_label.pack(side="bottom", pady=6)
def apply_theme():
    bg = "#1e1e1e" if dark_mode else "#f0f0f0"
    fg = "#ffffff" if dark_mode else "#000000"
    accent = "#2D89EF" if dark_mode else "#0078d7"
    entry_bg = "#2a2a2a" if dark_mode else "#ffffff"

    root.config(bg=bg)
    titlebar.config(bg=accent)
    title_label.config(bg=accent, fg="white")
    close_btn.config(bg=accent, fg="white", activebackground="red")
    frame.config(bg=bg)
    for widget in (browse_btn, install_btn, uninstall_btn, theme_toggle):
        widget.config(bg=accent, fg="white", activebackground="#444" if dark_mode else "#ccc")
    folder_entry.config(bg=entry_bg, fg=fg, insertbackground=fg)
    status_label.config(bg=bg, fg=fg)
apply_theme()
root.mainloop()
