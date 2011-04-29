#ifndef MAINSHIPCLASS_H
#define MAINSHIPCLASS_H

class mainShipClass {
public:
	mainShipClass() {}
	inline int getCurrentHealth() {
		return currentHealth;
	}
	inline int getMaxHealth() {
		return maxHealth;
	}
private:
	int currentHealth;
	int maxHealth;
};

#endif