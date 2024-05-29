use ahc015::solve1::Board;

#[test]
fn test_put() {
	let mut board = Board::new(vec![0; 10]);
	board.put(0, 0, 1);
	assert_eq!(board.board[0][0], 1);
}

#[test]
fn test_op_forward() {
	let mut board = Board::new(vec![0; 10]);

	board.put(8, 6, 1);
	board.put(8, 7, 1);
	board.put(9, 8, 1);
	board.op_forward();

	let answer = vec![
		vec![0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	];

	assert_eq!(board.board, answer);
}

#[test]
fn test_op_backward() {
	let mut board = Board::new(vec![0; 10]);

	board.put(8, 6, 1);
	board.put(8, 7, 1);
	board.put(9, 8, 1);
	board.op_backward();

	let answer = vec![
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
	];

	assert_eq!(board.board, answer);
}

#[test]
fn test_op_right() {
	let mut board = Board::new(vec![0; 10]);

	board.put(8, 6, 1);
	board.put(8, 7, 1);
	board.put(9, 8, 1);
	board.put(0, 8, 2);
	board.op_right();

	let answer = vec![
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 2, 1],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	];

	assert_eq!(board.board, answer);
}

#[test]
fn test_op_left() {
	let mut board = Board::new(vec![0; 10]);

	board.put(8, 6, 1);
	board.put(8, 7, 1);
	board.put(9, 8, 1);
	board.put(0, 8, 2);
	board.op_left();

	let answer = vec![
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![2, 1, 0, 0, 0, 0, 0, 0, 0, 0],
		vec![0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	];

	assert_eq!(board.board, answer);
}
