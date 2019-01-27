#include <test.ilh>

function main(argc:int, argv:**char):int {
	var total:int = 0;
	while (1) {
		while (1) {
			while (1) {
				while (1) {
					while (1) {
						while (1) {
							while (1) {
								while (1) {
									while (1) {
										while (1) {
											++total;
											break;
											return 1;
										}
										++total;
										break;
										return 1;
									}
									++total;
									break;
									return 1;
								}
								++total;
								break;
								return 1;
							}
							++total;
							break;
							return 1;
						}
						++total;
						break;
						return 1;
					}
					++total;
					break;
					return 1;
				}
				++total;
				break;
				return 1;
			}
			++total;
			break;
			return 1;
		}
		++total;
		break;
		return 1;
	}
	if (total != 10) {
		return 1;
	}

	total = 0;
	for (;;) {
		for (;;) {
			for (;;) {
				for (;;) {
					for (;;) {
						for (;;) {
							for (;;) {
								for (;;) {
									for (;;) {
										for (;;) {
											++total;
											break;
											return 1;
										}
										++total;
										break;
										return 1;
									}
									++total;
									break;
									return 1;
								}
								++total;
								break;
								return 1;
							}
							++total;
							break;
							return 1;
						}
						++total;
						break;
						return 1;
					}
					++total;
					break;
					return 1;
				}
				++total;
				break;
				return 1;
			}
			++total;
			break;
			return 1;
		}
		++total;
		break;
		return 1;
	}
	if (total != 10) {
		return 1;
	}

	total = 0;
	do {
		do {
			do {
				do {
					do {
						do {
							do {
								do {
									do {
										do {
											++total;
											break;
											return 1;
										} while (1);
										++total;
										break;
										return 1;
									} while (1);
									++total;
									break;
									return 1;
								} while (1);
								++total;
								break;
								return 1;
							} while (1);
							++total;
							break;
							return 1;
						} while (1);
						++total;
						break;
						return 1;
					} while (1);
					++total;
					break;
					return 1;
				} while (1);
				++total;
				break;
				return 1;
			} while (1);
			++total;
			break;
			return 1;
		} while (1);
		++total;
		break;
		return 1;
	} while (1);
	if (total != 10) {
		return 1;
	}

	return 0;
}