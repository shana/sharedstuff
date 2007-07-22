import System.IO
import System.Drawing

def addColor (color as Color):
	i = 0
	
def turnCounterClockwise(dir as int) as int:
	i = 0

def turnClockwise(dir as int) as int:
	i = 0

def line (i as int, mark as int):
	i = 0

def tryfill():
	i = 0

def addBitmap(transparent as bool):
	i = 0

def compose():
	i = 0

def clip():
	i = 0


s = StreamReader("D:\\h3x\\icfp\\2007\\trunk\\dna2rna\\endo.rna")	
rna = s.ReadToEnd()
s.Close()

i = 0

bucket = []
dir = 1
mark = -1

while i < rna.Length:
	r = rna[i:i+7]
	print rna[i:i+7]
	
	if r == "PIPIIIC":
		addColor (Color.Black)
	
	elif r == "PIPIIIP":
		addColor (Color.Red)
	
	elif r == "PIPIICC":
		addColor (Color.Green)

	elif r == "PIPIICF":
		addColor (Color.Yellow)

	elif r == "PIPIICP":
		addColor (Color.Blue)

	elif r == "PIPIIFC":
		addColor (Color.Magenta)

	elif r == "PIPIIFF":
		addColor (Color.Cyan)

	elif r == "PIPIIPC":
		addColor (Color.White)

	elif r == "PIPIIPF":
		addColor (Color.Transparent)

	elif r == "PIPIIPP":
		addColor (Color.Brown)

	elif r == "PIIPICP":
		bucket.Clear()

	elif r == "PCCCCCP":
		dir = turnCounterClockwise(dir)		

	elif r == "PFFFFFP":
		dir = turnClockwise(dir)		

	elif r == "PCCIFFP":
		mark = i

	elif r == "PFFICCP":
		line (i, mark)

	elif r == "PIIPIIP":
		tryfill()

	elif r == "PCCPFFP":
		addBitmap(false)
	elif r == "PFFPCCP":
		compose()

	elif r == "PFFICCF":
		clip()

	i+=7
	
