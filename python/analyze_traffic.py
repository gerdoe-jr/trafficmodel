import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


def load_traffic_csv(csv_path: Path) -> pd.DataFrame:
    df = pd.read_csv(csv_path)

    required_columns = {"arrival_time_seconds", "size_bytes"}
    missing_columns = required_columns - set(df.columns)

    if missing_columns:
        raise ValueError(f"missing columns in {csv_path}: {sorted(missing_columns)}")

    df["arrival_time_seconds"] = pd.to_numeric(df["arrival_time_seconds"])
    df["size_bytes"] = pd.to_numeric(df["size_bytes"])
    df = df.sort_values("arrival_time_seconds").reset_index(drop=True)

    return df


def calculate_characteristics(
    df: pd.DataFrame, model_name: str
) -> dict[str, float | str | int]:
    simulation_time = float(df["arrival_time_seconds"].max()) if not df.empty else 0.0
    total_bits = float(df["size_bytes"].sum() * 8)

    delays = df["arrival_time_seconds"].diff().dropna()

    return {
        "model": model_name,
        "packet_count": int(len(df)),
        "average_packet_size_bytes": (
            float(df["size_bytes"].mean()) if not df.empty else 0.0
        ),
        "average_bitrate_bits_per_second": (
            total_bits / simulation_time if simulation_time > 0.0 else 0.0
        ),
        "average_interarrival_delay_seconds": (
            float(delays.mean()) if not delays.empty else 0.0
        ),
    }


def print_characteristics(characteristics: dict[str, float | str | int]) -> None:
    print(f"Model: {characteristics['model']}")
    print(f"Packet count: {characteristics['packet_count']}")
    print(
        f"Average packet size, bytes: {characteristics['average_packet_size_bytes']:.6g}"
    )
    print(
        f"Average bitrate, bit/s: {characteristics['average_bitrate_bits_per_second']:.6g}"
    )
    print(
        f"Average interarrival delay, s: {characteristics['average_interarrival_delay_seconds']:.6g}"
    )


def plot_packet_timeline(df: pd.DataFrame, model_name: str, output_dir: Path) -> None:
    plt.figure(figsize=(10, 5))
    plt.scatter(df["arrival_time_seconds"], df["size_bytes"], s=14, alpha=0.75)
    plt.xlabel("Arrival time, s")
    plt.ylabel("Packet size, bytes")
    plt.title(f"{model_name}: packet timeline")
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout(pad=2.0)

    output_path = output_dir / "timeline.png"
    plt.savefig(output_path, dpi=220, bbox_inches="tight")
    plt.close()
    print(f"Saved {output_path}")


def plot_delay_histogram(df: pd.DataFrame, model_name: str, output_dir: Path) -> None:
    delays = df["arrival_time_seconds"].diff().dropna()

    if delays.empty:
        print("Skip delay histogram: not enough packets")
        return

    plt.figure(figsize=(9, 5))
    plt.hist(delays, bins=30)
    plt.xlabel("Interarrival delay, s")
    plt.ylabel("Count")
    plt.title(f"{model_name}: interarrival delay histogram")
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout(pad=2.0)

    output_path = output_dir / "delay_histogram.png"
    plt.savefig(output_path, dpi=220, bbox_inches="tight")
    plt.close()
    print(f"Saved {output_path}")


def plot_size_histogram(df: pd.DataFrame, model_name: str, output_dir: Path) -> None:
    plt.figure(figsize=(9, 5))
    plt.hist(df["size_bytes"], bins=30)
    plt.xlabel("Packet size, bytes")
    plt.ylabel("Count")
    plt.title(f"{model_name}: packet size histogram")
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout(pad=2.0)

    output_path = output_dir / "size_histogram.png"
    plt.savefig(output_path, dpi=220, bbox_inches="tight")
    plt.close()
    print(f"Saved {output_path}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("csv", help="CSV file produced by trafficmodel")
    parser.add_argument("--model", default=None, help="Model name for plot titles")
    parser.add_argument("--out-dir", default="plots", help="Output directory for plots")

    args = parser.parse_args()

    csv_path = Path(args.csv)
    model_name = args.model or csv_path.stem.replace("_traffic", "")
    output_dir = Path(args.out_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    df = load_traffic_csv(csv_path)
    characteristics = calculate_characteristics(df, model_name)

    print_characteristics(characteristics)

    plot_packet_timeline(df, model_name, output_dir)
    plot_delay_histogram(df, model_name, output_dir)
    plot_size_histogram(df, model_name, output_dir)


if __name__ == "__main__":
    main()
