

struct HsvColorSubSpace {
	struct range {
		int high;
		int low;
	};

	range hue;
	range saturation;
	range value;
};