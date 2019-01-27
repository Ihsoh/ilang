#include <test.ilh>

function main(argc:int, argv:**char):int {
	var total:long = 0;
	var i1_1:int = 0;
	while (i1_1 < 10) {
		++i1_1;
		if (i1_1 % 2 == 0) {
			continue;
		}
		var i2:int = 0;
		while (i2 < 10) {
			++i2;
			if (i2 % 2 == 0) {
				continue;
			}
			var i3:int = 0;
			while (i3 < 10) {
				++i3;
				if (i3 % 2 == 0) {
					continue;
				}
				var i4:int = 0;
				while (i4 < 10) {
					++i4;
					if (i4 % 2 == 0) {
						continue;
					}
					var i5:int = 0;
					while (i5 < 10) {
						++i5;
						if (i5 % 2 == 0) {
							continue;
						}
						var i6:int = 0;
						while (i6 < 10) {
							++i6;
							if (i6 % 2 == 0) {
								continue;
							}
							var i7:int = 0;
							while (i7 < 10) {
								++i7;
								if (i7 % 2 == 0) {
									continue;
								}
								var i8:int = 0;
								while (i8 < 10) {
									++i8;
									if (i8 % 2 == 0) {
										continue;
									}
									var i9:int = 0;
									while (i9 < 10) {
										++i9;
										if (i9 % 2 == 0) {
											continue;
										}
										var i10:int = 0;
										while (i10 < 10) {
											++i10;
											if (i10 % 2 == 0) {
												continue;
											}
											++total;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (total != 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2) {
		return 1;
	}

	total = 0;
	var i1_2:int = 0;
	do {
		++i1_2;
		if (i1_2 % 2 == 0) {
			continue;
		}
		var i2:int = 0;
		do {
			++i2;
			if (i2 % 2 == 0) {
				continue;
			}
			var i3:int = 0;
			do {
				++i3;
				if (i3 % 2 == 0) {
					continue;
				}
				var i4:int = 0;
				do {
					++i4;
					if (i4 % 2 == 0) {
						continue;
					}
					var i5:int = 0;
					do {
						++i5;
						if (i5 % 2 == 0) {
							continue;
						}
						var i6:int = 0;
						do {
							++i6;
							if (i6 % 2 == 0) {
								continue;
							}
							var i7:int = 0;
							do {
								++i7;
								if (i7 % 2 == 0) {
									continue;
								}
								var i8:int = 0;
								do {
									++i8;
									if (i8 % 2 == 0) {
										continue;
									}
									var i9:int = 0;
									do {
										++i9;
										if (i9 % 2 == 0) {
											continue;
										}
										var i10:int = 0;
										do {
											++i10;
											if (i10 % 2 == 0) {
												continue;
											}
											++total;
										} while (i10 < 10);
									} while (i9 < 10);
								} while (i8 < 10);
							} while (i7 < 10);
						} while (i6 < 10);
					} while (i5 < 10);
				} while (i4 < 10);
			} while (i3 < 10);
		} while (i2 < 10);
	} while (i1_2 < 10);
	if (total != 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2) {
		return 1;
	}

	total = 0;
	for (var i1:int = 0; i1 < 10; ++i1) {
		if (i1 % 2 == 0) {
			continue;
		}
		for (var i2:int = 0; i2 < 10; ++i2) {
			if (i2 % 2 == 0) {
				continue;
			}
			for (var i3:int = 0; i3 < 10; ++i3) {
				if (i3 % 2 == 0) {
					continue;
				}
				for (var i4:int = 0; i4 < 10; ++i4) {
					if (i4 % 2 == 0) {
						continue;
					}
					for (var i5:int = 0; i5 < 10; ++i5) {
						if (i5 % 2 == 0) {
							continue;
						}
						for (var i6:int = 0; i6 < 10; ++i6) {
							if (i6 % 2 == 0) {
								continue;
							}
							for (var i7:int = 0; i7 < 10; ++i7) {
								if (i7 % 2 == 0) {
									continue;
								}
								for (var i8:int = 0; i8 < 10; ++i8) {
									if (i8 % 2 == 0) {
										continue;
									}
									for (var i9:int = 0; i9 < 10; ++i9) {
										if (i9 % 2 == 0) {
											continue;
										}
										for (var i10:int = 0; i10 < 10; ++i10) {
											if (i10 % 2 == 0) {
												continue;
											}
											++total;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (total != 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2
				* 10 / 2) {
		return 1;
	}

	return 0;
}