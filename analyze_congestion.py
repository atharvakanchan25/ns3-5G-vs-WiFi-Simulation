import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score

# Simulated network data
data = {
    "latency": [5, 10, 50, 2, 20, 100, 8, 30, 90, 5],
    "packet_loss": [0.01, 0.05, 0.20, 0.001, 0.1, 0.5, 0.02, 0.15, 0.4, 0.03],
    "throughput": [9, 8, 5, 10, 7, 2, 9, 6, 3, 8],
    "congestion": [0, 0, 1, 0, 0, 1, 0, 1, 1, 0]
}

df = pd.DataFrame(data)

# Split data
X = df[["latency", "packet_loss", "throughput"]]
y = df["congestion"]
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train AI model
model = RandomForestClassifier(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# Predict and evaluate
y_pred = model.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
print(f"Congestion Prediction Accuracy: {accuracy * 100:.2f}%")

# Test new network conditions
new_conditions = np.array([[30, 0.15, 6]])  # Example input
prediction = model.predict(new_conditions)
print("Predicted Congestion Level:", "High" if prediction[0] == 1 else "Normal")
